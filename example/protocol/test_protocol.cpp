#include "test_protocol.h"

TestProtocol::TestProtocol(ApacheServerRec *pServer)
    : ApacheProtocol(pServer)
{
}

TestProtocol::~TestProtocol()
{
}

int TestProtocol::process_connection(conn_rec *c)
{
    apr_bucket_brigade *bb;
    apr_bucket *b;
    char *buf;
    
    ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                 "PROTOCOL!!!!");
    buf = apr_psprintf(c->pool, "Hello! %s (%s %s) is alive!!!\n\n",
		       c->base_server->server_hostname,
		       ap_get_server_version(), 
		       ap_get_server_built());

    bb = apr_brigade_create(c->pool,
                            c->bucket_alloc);
    b = apr_bucket_pool_create(buf, strlen(buf), c->pool,
                               c->bucket_alloc);
    APR_BRIGADE_INSERT_HEAD(bb, b);
    b = apr_bucket_flush_create(c->bucket_alloc);
    APR_BRIGADE_INSERT_TAIL(bb, b);

    ap_pass_brigade(c->output_filters, bb);
    return DECLINED;
}

/*=====================INSTANCIATORS==========================*/

ApacheProtocol *instanciate_protocol(ApacheServerRec *pServer)
{
    return new TestProtocol(pServer);
}

cpp_factory_t test_protocol = {
    NULL,
    NULL,
    NULL,
    instanciate_protocol,
};

