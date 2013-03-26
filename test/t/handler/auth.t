use strict;
use warnings FATAL => 'all';

use Apache::Test;
use Apache::TestRequest;

sub test_auth
{
    return 1;
}

plan tests => 2, \&test_auth;

my $auth_url = '/cpp-auth';

ok GET_RC($auth_url) == 401;
ok GET_RC($auth_url, username => 'foo', password => 'bar') == '401';
#ok GET_RC($auth_url, username => 'sterling', password => 'bar');
