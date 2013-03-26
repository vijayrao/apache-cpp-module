#define EXPORT_MODCPP

#include "apr_dso.h"
#include "apache_handler.h"
#include "apache_protocol.h"
#include "apache_filters.h"
#include <memory>

extern "C" {
    
#define CALL_REQ_FUNCTION(function_name) \
    int result = DECLINED; \
    std::auto_ptr<ApacheRequestRec> pRequest(new ApacheRequestRec(r)); \
    cpp_config_rec *dir_rec = (cpp_config_rec *) \
        pRequest->get_dir_config(&cplusplus_module); \
    char **cur_handler = dir_rec->handlers; \
    while( cur_handler != NULL && *cur_handler != NULL ) { \
        ApacheHandler *handler = get_handler(pRequest.get(), *cur_handler); \
        if( !handler ) { \
            result = DECLINED; \
        } else { \
            result = handler->function_name(pRequest.get()); \
        } \
        if( result != DECLINED ) { \
            break; \
        } \
        cur_handler++; \
    } \
    return result;
    
    ApacheHandler *get_handler(ApacheRequestRec *pRequest, char *name)
    {
        cpp_server_rec *server_rec = NULL;
        
        if( !name ) {
            return NULL;
        }
        server_rec = (cpp_server_rec *)
            pRequest->server()->get_module_config(&cplusplus_module);
        
        ApacheHandler *handler = (ApacheHandler *)
            apr_hash_get(server_rec->handler_hash, 
                         name,
                         strlen(name));
        return handler;
    }
    
    int cpp_call_handler(request_rec *r)
    {
        CALL_REQ_FUNCTION(handler);
    }
    
    int cpp_call_fixups(request_rec *r)
    {
        CALL_REQ_FUNCTION(fixups);
    }
    
    int cpp_call_post_read_request(request_rec *r)
    {
        CALL_REQ_FUNCTION(post_read_request);
    }
    
    int cpp_call_translate_name(request_rec *r)
    {
        CALL_REQ_FUNCTION(translate_name);
    }
    
    int cpp_call_header_parser(request_rec *r)
    {
        CALL_REQ_FUNCTION(header_parser);
    }
    
    int cpp_call_access_checker(request_rec *r)
    {
        CALL_REQ_FUNCTION(access_checker);
    }
    
    int cpp_call_auth_checker(request_rec *r)
    {
        CALL_REQ_FUNCTION(auth_checker);
    }
    
    int cpp_call_check_user_id(request_rec *r)
    {
        CALL_REQ_FUNCTION(check_user_id);
    }
    
    int cpp_call_type_checker(request_rec *r)
    {
        CALL_REQ_FUNCTION(type_checker);
    }
    
    int cpp_call_logger(request_rec *r)
    {
        CALL_REQ_FUNCTION(logger);
    }

    apr_status_t delete_handler(apr_hash_index_t *current_entry)
    {
        void *val;
        apr_hash_this(current_entry, NULL, NULL, &val);
        ApacheHandler *handler = (ApacheHandler *)val;
        delete handler;
        return APR_SUCCESS;
    }
    static apr_status_t delete_input_filter(void *data)
    {
        ApacheInputFilter *filter = (ApacheInputFilter *)data;
        delete filter;
        return APR_SUCCESS;
    }
    static apr_status_t delete_output_filter(void *data)
    {
        ApacheOutputFilter *filter = (ApacheOutputFilter *)data;
        delete filter;
        return APR_SUCCESS;
    }
    static apr_status_t delete_protocol_handler(void *data)
    {
        ApacheProtocol *protocol = (ApacheProtocol *)data;
        delete protocol;
        return APR_SUCCESS;
    }
    
    char *load_cpp_module( apr_pool_t *pool, server_rec *server, cpp_server_rec *server_rec, apr_hash_index_t *next)
    {
        apr_dso_handle_t *sohandle;
        apr_dso_handle_sym_t sosymbol;
	ApacheServerRec *pServer = new ApacheServerRec(server);
        cpp_factory_t *cur_handler;
	apr_ssize_t n_len;
	const void *n;
	void *p;
        char *name;
	char *path;
	apr_hash_this(next, &n, &n_len, &p);
	name = apr_pstrndup(pool, (char *)n, n_len);
        path = apr_pstrdup(pool, (char *)p);

        if ( apr_dso_load(&sohandle, path, pool) != APR_SUCCESS ) {
            char my_error[256];
            return apr_pstrcat( 
                pool, "Error Loading CPP SO ", path, " into server: ",
                apr_dso_error( sohandle, my_error, sizeof(my_error)),
                NULL);
        }

        /* Log the event */
        ap_log_perror( APLOG_MARK, APLOG_DEBUG|APLOG_NOERRNO, 
                       0, pool, 
                       "loaded CPP so: %s", name);

        if ( apr_dso_sym ( &sosymbol, sohandle, name) != APR_SUCCESS) {
            char my_error[256];
            return apr_pstrcat(
                pool, "Can't locate cpp_factory_t `", 
                name, "' in file ", path, ": ", 
                apr_dso_error(sohandle, my_error, sizeof(my_error)),
                NULL);
        }

        /* symbol should now be ref the factory structure with
         * the handler, input, output and protocol filters
         */
        cur_handler = (cpp_factory_t *) sosymbol;

        ApacheHandler *handler = cur_handler->handler_func ? 
            cur_handler->handler_func(pServer) : NULL;
        ApacheInputFilter *input_filter = cur_handler->input_filter_func ?
            cur_handler->input_filter_func(pServer) : NULL;
        ApacheOutputFilter *output_filter = cur_handler->output_filter_func ?
            cur_handler->output_filter_func(pServer) : NULL;
        ApacheProtocol *protocol = cur_handler->protocol_func ?
            cur_handler->protocol_func(pServer) : NULL;
        
        if( handler != NULL ) {
            apr_hash_set(server_rec->handler_hash, 
                         name, strlen(name), handler);
        }
        if( input_filter != NULL ) {
            apr_hash_set(server_rec->input_filter_hash, 
                         name, strlen(name), input_filter);
            apr_pool_cleanup_register(pool, (void *)input_filter, delete_input_filter,
                                      apr_pool_cleanup_null);
        }
        if( output_filter != NULL ) {
            apr_hash_set(server_rec->output_filter_hash, 
                         name, strlen(name), output_filter);
            apr_pool_cleanup_register(pool, (void *)output_filter, delete_output_filter,
                                      apr_pool_cleanup_null);
        }
        if( protocol != NULL ) {
            apr_hash_set(server_rec->protocol_hash, 
                         name, strlen(name), protocol);
            apr_pool_cleanup_register(pool, (void *)protocol, delete_protocol_handler,
                                      apr_pool_cleanup_null);
        }
        return NULL;
    }
}

char *ApacheBase::get_server_var(ApacheServerRec *pServer, const char *name)
{
    cpp_server_rec *server_rec = NULL;
    if( !name ) {
        return NULL;
    }
    server_rec = (cpp_server_rec *)
                 pServer->get_module_config(&cplusplus_module);    
    char *value = (char *)apr_hash_get(server_rec->var_hash, name, strlen(name));
    return value;
}

char *ApacheBase::get_cpp_var(ApacheRequestRec *pRequest, const char *name)
{
    cpp_config_rec *dir_rec = (cpp_config_rec *)
        pRequest->get_dir_config(&cplusplus_module);
    char *value = (char *)apr_hash_get(dir_rec->var_hash, name, strlen(name));
    return value;
}
