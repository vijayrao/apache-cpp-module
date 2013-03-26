#include "apache_protocol.h"

class TestProtocol : public ApacheProtocol
{
public:
  
    TestProtocol(ApacheServerRec *pServer);
    ~TestProtocol(void);
    int process_connection(conn_rec *c);
};
