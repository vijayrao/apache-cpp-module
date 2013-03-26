package Apache::TestSmoke;

use strict;
use warnings FATAL => 'all';

use Apache::Test ();
use Apache::TestConfig ();
use Apache::TestTrace;

use Getopt::Long qw(GetOptions);
use File::Spec::Functions qw(catfile);
use FindBin;

#use constant DEBUG => 1;

# how many times to run all tests at the first iteration
use constant DEFAULT_TIMES  => 10;

# how many various seeds to try in NONSTOP mode
use constant DEFAULT_ITERATIONS  => 10;

# if after this number of tries to reduce the number of tests fails we
# give up on more tries
use constant MAX_REDUCTION_TRIES => 50;

my @num_opts  = qw(times iterations);
my @string_opts  = qw(order report);
my @flag_opts = qw(help);

my %order = map {$_ => 1} qw(random repeat rotate);

my %usage = (
   'iterations=N'    => 'number of random iterations to run' .
                        ' (default: ' . DEFAULT_ITERATIONS . ')',
   'times=N'         => 'try to repeat all tests at most N times' .
                        ' (default: ' . DEFAULT_TIMES . ')',
   'order=MODE'      => 'modes: random, repeat, rotate' .
                        ' (default: random)',
   'report=FILENAME' => 'save report in a filename' .
                        ' (default: smoke-report-<date>.txt)',
);

sub new {
    my($class, @argv) = @_;

    my $self = bless {
        seen    => {}, # seen sequences and tried them md5 hash
        results => {}, # final reduced sequences md5 hash
        smoking_completed         => 0,
        start_with_tests          => [],
        total_iterations          => 0,
        total_reduction_attempts  => 0,
        total_reduction_successes => 0,
        total_tests_run           => 0,
    }, ref($class)||$class;

    $self->getopts(\@argv);
    my $opts = $self->{opts};

    chdir "$FindBin::Bin/..";

    $self->{times} = $opts->{times} || DEFAULT_TIMES;
    $self->{order} = $opts->{order} || 'random';

    # it doesn't make sense to run a known sequence more than once
    if ($self->{order} eq 'random') {
        $self->{run_iter} = $opts->{iterations} || DEFAULT_ITERATIONS;
    }
    else {
        error "forcing only one iteration for non-random order";
        $self->{run_iter} = 1;
    }

    $self->{base_command}    = "t/TEST";
    $self->{first_time_args} = "-order=$self->{order} -times=$self->{times}";

    $self->{start_command} = join " ",
        $self->{base_command},
        $self->{first_time_args},
        @{ $self->{start_with_tests} };

    $self;
}

sub getopts {
    my($self, $argv) = @_;
    my %opts;
    local *ARGV = $argv;

    # permute      : optional values can come before the options
    # pass_through : all unknown things are to be left in @ARGV
    Getopt::Long::Configure(qw(pass_through permute));

    # grab from @ARGV only the options that we expect
    GetOptions(\%opts, @flag_opts,
               (map "$_=s", @string_opts),
               (map "$_=i", @num_opts));

    if (exists $opts{order}  && !exists $order{$opts{order}}) {
        error "unknown -order mode: $opts{order}";
        $self->opt_help();
        exit;
    }

    if ($opts{help}) {
        $self->opt_help;
        exit;
    }

    # copy ARGV away
    push @{ $self->{start_with_tests} }, @ARGV if @ARGV;

    # min
    $self->{opts} = \%opts;
}


sub run {
    my($self) = shift;

    # make sure that there the server is down
    $self->kill_proc();

    $self->report_start();
    my $iter = 0;
    while ($iter++ < $self->{run_iter}) {
        $self->run_iter($iter);
    }
    $self->{smoking_completed} = 1;
    $self->report_finish();
    exit;
}

sub sep {
    my ($char, $title) = @_;
    my $width = 60;
    if ($title) {
        my $side = int( ($width - length($title) - 2) / 2);
        my $pad  = ($side+1) * 2 + length($title) < $width ? 1 : 0;
        return $char x $side . " $title " . $char x ($side+$pad);
    }
    else {
        return $char x $width;
    }
}

sub run_iter {
    my($self, $iter) = @_;

    my $reduce_iter = 0;
    my @good = ();
    warning "\n" . sep("-");
    warning sprintf "[%03d-%02d-%02d] trying all tests $self->{times} times",
        $iter, $reduce_iter, 0;

    my $command = $self->{start_command};

    # first time run all tests (so we don't specify them)
    my $bad = $self->run_test($iter, $reduce_iter, $command, \@good);
    unless ($bad) {
        $self->{total_iterations}++;
        return;
    }
    error "recorded a positive failure, will try to minimize the input now";

    # positive failure
    my $ok_tests = @good;
    $command = $self->{base_command};
    my $reduction_success = 0;
    my $done = 0;
    while (@good > 1) {
        my $tries = 0;
        my $reduce_sub = $self->reduce_stream(\@good);
        $reduce_iter++;
        while ($tries++ < MAX_REDUCTION_TRIES) {
            $self->{total_reduction_attempts}++;
            my @try = @{ $reduce_sub->() };

            # reduction stream is empty (tried all?)
            unless (@try) {
                $done = 1;
                last;
            }

            my $try_command = "$command @try $bad";
            warning sprintf "\n[%03d-%02d-%02d] trying %d tests",
                $iter, $reduce_iter, $tries, scalar(@try);
            my @ok = ();
            my $new_bad = $self->run_test($iter, $reduce_iter,
                                          $try_command, \@ok);
            if ($new_bad) {
                # successful reduction
                $reduction_success++;
                @good = @ok;
                $tries = 0;
                my $num = @ok;
                error "*** reduction $reduce_iter succeeded ($num tests) ***";
                $self->{total_reduction_successes}++;
                last;
            }
        }

        # last round of reducing has failed, so we give up
        if ($done || $tries >= MAX_REDUCTION_TRIES){
            error "no further reductions were made";
            $done = 1;
            last;
        }

    }

    # we have a minimal failure sequence at this point (to the extend
    # of success of our attempts to reduce)

    # report the sequence if we didn't see such one yet in the
    # previous iterations
    unless ($self->sequence_seen($self->{results}, [@good, $bad])) {
        # if no reduction succeeded, it's 0
        $reduce_iter = 0 unless $reduction_success;
        $self->report_success($iter, $reduce_iter,
                              "$command @good $bad", @good + 1);
    }

    $self->{total_iterations}++;

}

# my $sub = $self->reduce_stream(\@items);
sub reduce_stream {
    my($self) = shift;
    my @items = @{+shift};

    my $items = @items;
    my $odd   = $items % 2 ? 1 : 0;
    my $middle = int($items/2) - 1;
    my $c = 0;

    return sub {
        $c++; # remember stream's state 

        # a single item is not reduce-able
        return \@items if $items == 1;

        my @try = ();
        my $max_repeat_tries = 50; # avoid seen sequences
        my $repeat = 0;
        while ($repeat++ <= $max_repeat_tries) {

            # try to use a binary search
            if ($c == 1) {
                # right half
                @try = @items[($middle+1)..($items-1)];
            }
            elsif ($c == 2) {
                # left half
                @try = @items[0..$middle];
            }

            # try to use a random window size alg
            else {
                my $left = int rand($items);
                $left = $items - 1 if $left == $items - 1;
                my $right = $left + int rand($items - $left);
                $right = $items - 1 if $right >= $items;
                @try = @items[$left..$right];
            }

            if ($self->sequence_seen($self->{seen}, \@try)) {
                @try = ();
            }
            else {
                last; # found an unseen sequence
            }
        }
        return \@try;
    }
}

sub sequence_seen {
    my ($self, $rh_store, $ra_tests) = @_;

    require Digest::MD5;
    my $digest = Digest::MD5::md5_hex(join '', @$ra_tests);
    #error $self->{seen};
    return $rh_store->{$digest}++ ? 1 : 0

}

sub run_test {
    my($self, $iter, $count, $command, $ra_ok) = @_;
    my $bad = '';

    warning $command;

    #$SIG{PIPE} = 'IGNORE';
    $SIG{PIPE} = sub { die "pipe broke" };
    open my $pipe, "$command 2>&1|" or die "cannot fork: $!";
    my $oldfh = select $pipe; $| = 1; select $oldfh;

    while (my $t = <$pipe>) {
        next unless $t =~ /^(\S+?)\.+(ok|FAILED)/;
        $self->{total_tests_run}++;
        push(@$ra_ok, $1), next if $2 eq 'ok';

        # failure
        $bad = $1;
#error "$1: $2";
        last;
    }
    # it's normal for $command to exit with a failure status if tests
    # fail, so we don't die/report it
    close $pipe; 
    $self->kill_proc();
    return $bad;

}

sub report_start {
    my($self) = shift;

    my $time = scalar localtime;
    $self->{start_time} = $time;
    $time =~ s/\s/_/g;
    $time =~ s/:/-/g; # winFU
    my $file = $self->{opts}->{report} || "smoke-report-$time.txt";

    open my $fh, ">$file" or die "cannot open $file for writing: $!";
    $self->{fh} = $fh;
    my $sep = sep("-");
    my $title = sep('=', "Special Tests Sequence Failure Finder Report");

        print $fh <<EOM;
$title
$sep
First iteration used:
$self->{start_command}
$sep
EOM

    $SIG{INT} = sub {
        $self->report_finish;
        exit;
    };

}

sub report_success {
    my($self, $iter, $reduce_iter, $sequence, $tests) = @_;

    my @report = ("iteration $iter ($tests tests):\n",
        "\t$sequence\n",
        "(made $reduce_iter successful reductions)\n\n");

    print @report;
    if (my $fh = $self->{fh}) {
        print $fh @report;
    }
}

sub report_finish {
    my($self) = @_;

    my $start_time = $self->{start_time};
    my $end_time   = scalar localtime;
    if (my $fh = delete $self->{fh}) {
        my $failures = scalar keys %{ $self->{results} };

        my $sep = sep("-");
        my $cfg_as_string = $self->build_config_as_string;
        my $unique_seqs   = scalar keys %{ $self->{results} };
        my $attempts      = $self->{total_reduction_attempts};
        my $successes     = $self->{total_reduction_successes};
        my $completion    = $self->{smoking_completed} 
            ? "Completed"
            : "Not Completed (aborted by user)";

        my $status = "Unknown";
        if ($self->{total_iterations} > 0) {
            if ($failures) {
                $status = "*** NOT OK ***";
            }
            else {
                $status = "+++ OK +++";
            }
        }

        my $title = sep('=', "Summary");
        print $fh <<EOM;

$title
Completion              : $completion
Status                  : $status
Tests run               : $self->{total_tests_run}
Random iterations made  : $self->{total_iterations}
EOM

        if ($attempts > 0 && $failures) {
            my $reduction_stats = sprintf "%d/%d (%d%% success)",
                $attempts, $successes, $successes / $attempts * 100;

            print $fh <<EOM;
Unique sequences found  : $unique_seqs
Reduction tries/success : $reduction_stats
EOM
        }

        print $fh <<EOM;
$sep
--- Started at: $start_time ---
--- Ended   at: $end_time ---
$sep
The smoke testing was run on the system with the following
parameters:

$cfg_as_string

-- this report was generated by $0
EOM
        close $fh;
    }
}

sub build_config_as_string {
    Apache::TestConfig::as_string();
}

sub kill_proc {
    my($self) = @_;

    # a hack
    my $file = "t/logs/httpd.pid";
    return unless -f $file;
    my $pid = `cat $file`;
    chomp $pid;
    return unless $pid;
    kill SIGINT => $pid;
}

sub opt_help {
    my $self = shift;

    print <<EOM;
usage: t/SMOKE [options ...] [tests]
    where the options are:
EOM

    for (sort keys %usage){
        printf "   -%-16s %s\n", $_, $usage{$_};
    }
    print <<EOM;

    if 'tests' argument is not provided all available tests will be run
EOM
}

# generate t/SMOKE script (or a different filename) which will drive
# Apache::TestSmoke
sub generate_script {
    my ($class, $file) = @_;

    $file ||= catfile 't', 'SMOKE';

    my $content = join "\n",
        "BEGIN { eval { require blib; } }",
        Apache::TestConfig->modperl_2_inc_fixup,
        Apache::TestConfig->perlscript_header,
        "use $class;",
        "$class->new(\@ARGV)->run;";

    Apache::Test::config()->write_perlscript($file, $content);
}

1;
__END__

=head1 NAME

Apache::TestSmoke - Special Tests Sequence Failure Finder

=head1 SYNOPSIS

  # get the usage and the default values
  % t/SMOKE -help

  # repeat all tests 5 times and try 20 random iterations
  # and save the report into the file 'myreport'
  % t/SMOKE -times=5 -iterations=20 -report=myreport

  # run all tests default number of iterations, and repeat tests
  # default number of times
  % t/SMOKE

  # same as above but work only the specified tests
  % t/SMOKE foo/bar foo/tar

  # run once a sequence of tests in a non-random mode
  # e.g. when trying to reduce a known long sequence that fails
  % t/SMOKE -order=rotate -times=1 foo/bar foo/tar

  # now read the created report file

=head1 DESCRIPTION

=head2 The Problem

When we try to test a stateless machine (i.e. all tests are
independent), running all tests once ensures that all tested things
properly work. However when a state machine is tested (i.e. where a
run of one test may influence another test) it's not enough to run all
the tests once to know that the tested features actually work. It's
quite possible that if the same tests are run in a different order
and/or repeated a few times, some tests may fail.  This usually
happens when some tests don't restore the system under test to its
pristine state at the end of the run, which may influence other tests
which rely on the fact that they start on pristine state, when in fact
it's not true anymore. In fact it's possible that a single test may
fail when run twice or three times in a sequence.

=head2 The Solution

To reduce the possibility of such dependency errors, it's helpful to
run random testing repeated many times with many different srand
seeds. Of course if no failures get spotted that doesn't mean that
there are no tests inter-dependencies, which may cause a failure in
production. But random testing definitely helps to spot many problems
and can give better test coverage.

=head2 Resolving Sequence Problems

When this kind of testing is used and a failure is detected there are
two problems:

=over

=item 1

First is to be able to reproduce the problem so if we think we fixed
it, we could verify the fix. This one is easy, just remember the
sequence of tests run till the failed test and rerun the same sequence
once again after the problem has been fixed.

=item 2

Second is to be able to understand the cause of the problem. If during
the random test the failure has happened after running 400 tests, how
can we possibly know which previously running tests has caused to the
failure of the test 401. Chances are that most of the tests were clean
and don't have inter-dependency problem. Therefore it'd be very
helpful if we could reduce the long sequence to a minimum. Preferably
1 or 2 tests. That's when we can try to understand the cause of the
detected problem.

=back

This utility attempts to solve both problems, and at the end of each
iteration print a minimal sequence of tests causing to a failure. This
doesn't always succeed, but works in many cases.

This utility:

=over

=item 1

Runs the tests randomly until the first failure is detected. Or
non-randomly if the option I<-order> is set to I<repeat> or I<rotate>.

=item 2

Then it tries to reduce that sequence of tests to a minimum, and this
sequence still causes to the same failure.

=item 3

(XXX: todo): then it reruns the minimal sequence in the verbose mode
and saves the output.

=item 4

It reports all the successful reductions as it goes to STDOUT and
report file of the format: smoke-report-<date>.txt.

In addition the systems build parameters are logged into the report
file, so the detected problems could be reproduced.

=item 5

Goto 1 and run again using a new random seed, which potentially should
detect different failures.

=back

=head1 Reduction Algorithm

Currently for each reduction path, the following reduction algorithms
get applied:

=over

=item 1

Binary search: first try the upper half then the lower.

=item 2

Random window: randomize the left item, then the right item and return
the items between these two points.

=back

=head1 t/SMOKE.PL

I<t/SMOKE.PL> is driving this module, if you don't have it, create it:

  #!perl
  
  use strict;
  use warnings FATAL => 'all';
  
  use FindBin;
  use lib "$FindBin::Bin/../Apache-Test/lib";
  use lib "$FindBin::Bin/../lib";
  
  use Apache::TestSmoke ();
  
  Apache::TestSmoke->new(@ARGV)->run;

usually I<Makefile.PL> converts it into I<t/SMOKE> while adjusting the
perl path, but you create I<t/SMOKE> in first place as well.

=head1 AUTHOR

Stas Bekman

=cut
