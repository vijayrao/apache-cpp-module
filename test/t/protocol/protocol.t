use strict;
use warnings FATAL => 'all';

use Apache::Test;
use Apache::TestRequest;
use IO::Socket;

sub test_handler
{
    return 1;
}

sub test_protocol
{
    my $socket = IO::Socket::INET->new(PeerAddr => "127.0.0.1",
                                       PeerPort => 8528,
                                       Proto => "tcp",
                                       Type => SOCK_STREAM);
    my $response;
    print $socket "GET\n\n";
    $response = <$socket>;
    if( $response =~ /^Hello! / ) {
        return 1;
    }
    else {
        print STDOUT "\n\n".$response."\n\n";
        print STDERR "\n\n".$response."\n\n";
        return 0;
    }
}                    

plan tests => 1, \&test_handler;

ok &test_protocol(); 
