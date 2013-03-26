#include "test_handler.h"


TestHandler::TestHandler(ApacheServerRec *pServer)
:    ApacheHandler(pServer)
{
    mHits = 0;
    ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                 "constructing mod_cplusplus handler.");
}

TestHandler::~TestHandler()
{
    ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                 "destroying mod_cplusplus handler.");
}

int TestHandler::handler(ApacheRequestRec *pRequest)
{
    mHits++;
    pRequest->content_type("application/x-httpd-cgi");
    pRequest->dump();
    pRequest->rprintf("BOO");
    ap_setup_client_block(pRequest->get_request_rec(), REQUEST_CHUNKED_ERROR);
    return OK;
}

/*=====================INSTANCIATORS==========================*/

ApacheHandler *instanciate_test(ApacheServerRec *pServer)
{
    return new TestHandler(pServer);
}

cpp_factory_t test_handler = {
    instanciate_test,
    NULL,
    NULL,
    NULL,
};
