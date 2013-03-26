use strict;
use warnings FATAL => 'all';

use Apache::Test;
use Apache::TestRequest;

sub test_input
{
    return 1;
}

plan tests => 1, \&test_input;

my $input_url = '/cpp-input';

ok GET_BODY($input_url) =~ "host";
