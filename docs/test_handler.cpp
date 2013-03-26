#include "apache_handler.h"

class TestHandler : public ApacheHandler
{
public:
    TestHandler(void) { };
    ~TestHandler(void) { };
    int handler(ApacheRequestRec *pRequest);
};


int TestHandler::handler(ApacheRequestRec *pRequest)
{
    pRequest->rprintf("\nThis handler is called for user %s", pRequest->user());
    pRequest->rputs("\nLets Dump The Request!\n");
    pRequest->dump();
    return OK;
}

