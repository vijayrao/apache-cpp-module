use strict;
use warnings FATAL => 'all';

use Apache::Test;
use Apache::TestRequest;

sub test_output
{
    return 1;
}

sub reverse
{
    my @content = split('', shift);
    my @new_content;
    my $len = @content - 1;
    my $idx = 0;
    while( @content )
    {
        unshift(@new_content, shift(@content));
        $idx++;
    }
    return join('', @new_content);
}
plan tests => 2, \&test_output;

my $output_url = '/cpp-output';
my $handler_url = '/cpp-handler';
ok !(GET_BODY($output_url) =~ 'BOO');
ok (GET_BODY($output_url) =~ 'OOB');


