#include "test_input.h"
#include "ctype.h"

TestInputFilter::TestInputFilter(ApacheServerRec *pServer)
    : ApacheInputFilter(pServer)
{
       pszToLower = get_server_var(pServer, "tolower");
        ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                 "done retrieving param");
        if( pszToLower == NULL ) {
            ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL, "didn't get cpp var parameter");
        }
}

TestInputFilter::~TestInputFilter()
{
}

apr_status_t TestInputFilter::connection_input_filter(ap_filter_t *f, 
                                                      apr_bucket_brigade *bb,
                                                      ap_input_mode_t eMode, 
                                                      apr_read_type_e eBlock,
                                                      apr_off_t readbytes)
{
    ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                 "EXAMPLE modcplusplus input filter!!!!");

    apr_bucket *input_bucket;
    apr_bucket *output_bucket;
    
    apr_bucket_brigade *input_brigade = 
        apr_brigade_create(f->c->pool,
                           f->c->bucket_alloc);
    apr_status_t ret = ap_get_brigade(f->next, input_brigade, eMode, eBlock, 
                                      readbytes);
    if(ret != APR_SUCCESS) {
        return ret;
    }
    while(!APR_BRIGADE_EMPTY(input_brigade)) {
        input_bucket = APR_BRIGADE_FIRST(input_brigade);
        const char *data;
        apr_size_t len;
        
        if(APR_BUCKET_IS_EOS(input_bucket)) {
            APR_BUCKET_REMOVE(input_bucket);
            APR_BRIGADE_INSERT_TAIL(bb, input_bucket);
            break;
        }
        
        ret = apr_bucket_read(input_bucket, &data, &len, APR_BLOCK_READ);
        char *new_buf = apr_pstrndup(f->c->pool, data, len);
        if(ret != APR_SUCCESS) {
            return ret;
        }

        ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                 "XXX looking up host param");    
        for(unsigned int n=0 ; n < len ; ++n) {
            if(!strncmp(&data[n], pszToLower, strlen(pszToLower)))
                new_buf[n]=tolower(data[n]);
            else
                new_buf[n] = data[n];
        }
        ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                     "crazy content: %s (%lu)", new_buf, (unsigned long)len);
        output_bucket = apr_bucket_heap_create(new_buf, len, NULL,
                                               f->c->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(bb, output_bucket);
        apr_bucket_delete(input_bucket);
    }
    return APR_SUCCESS;
}

/*=====================INSTANTIATORS==========================*/

ApacheInputFilter *instanciate_input(ApacheServerRec *pServer)
{
    return new TestInputFilter(pServer);
}

cpp_factory_t test_input_filter = {
    NULL,
    instanciate_input,
    NULL,
    NULL,
};
