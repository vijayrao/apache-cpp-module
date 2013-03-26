use strict;
use warnings FATAL => 'all';

use Apache::Test;
use Apache::TestRequest;

sub test_handler
{
    return 1;
}

plan tests => 1, \&test_handler;

my $handler_url = '/cpp-handler';

ok GET_BODY($handler_url) =~ "BOO";
