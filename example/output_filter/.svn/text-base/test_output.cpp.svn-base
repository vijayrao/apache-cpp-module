#include "test_output.h"

TestOutputFilter::TestOutputFilter(ApacheServerRec *pServer)
    : ApacheOutputFilter(pServer)
{
}

TestOutputFilter::~TestOutputFilter()
{
}

int TestOutputFilter::connection_output_filter(ap_filter_t *f,
                                                apr_bucket_brigade *b) {
    ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL, "EEGAH - output connection filter test.");
    return ap_pass_brigade(f->next, b); 
}
    
apr_status_t TestOutputFilter::request_output_filter(ap_filter_t *f, 
                                             apr_bucket_brigade *bb)
{
    ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                 "EXAMPLE modcplusplus output filter!!!!");
  
    apr_bucket *input_bucket;
    apr_bucket_brigade *new_brigade;

    new_brigade = apr_brigade_create(f->r->pool,
                                     f->c->bucket_alloc);
    for( input_bucket = APR_BRIGADE_FIRST(bb);
           input_bucket != APR_BRIGADE_SENTINEL(bb);
               input_bucket = APR_BUCKET_NEXT(input_bucket) ) {
        const char *data;
        apr_size_t len;
        char *buf = NULL;
        apr_size_t n;
        apr_bucket *new_bucket;

        if(APR_BUCKET_IS_EOS(input_bucket)) {
            apr_bucket *eos_bucket = apr_bucket_eos_create(f->c->bucket_alloc);
            APR_BRIGADE_INSERT_TAIL(new_brigade, eos_bucket);
            break;
        }

        apr_bucket_read(input_bucket, &data, &len, APR_BLOCK_READ);
        
        buf = (char *)apr_pcalloc(f->r->pool, len);
        for(n = 0; n < len ; ++n) {
            buf[n] = data[len - n -1];
        }
        
	new_bucket = apr_bucket_pool_create(buf,len,f->r->pool,
                                            f->c->bucket_alloc);
	APR_BRIGADE_INSERT_TAIL(new_brigade, new_bucket);
    }
    return ap_pass_brigade(f->next, new_brigade);
}

ApacheOutputFilter *instanciate_output(ApacheServerRec *pServer)
{
    return new TestOutputFilter(pServer);
}

cpp_factory_t test_output_filter = {
    NULL,
    NULL,
    instanciate_output,
    NULL,
};
