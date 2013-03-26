#define EXPORT_MODCPP

#include "apache_filters.h"

extern "C" {
    apr_status_t cpp_call_input_filter(ap_filter_t *f,
                                       apr_bucket_brigade *b,
                                       ap_input_mode_t eMode,
                                       apr_read_type_e eBlock,
                                       apr_off_t readbytes)
    {
        ApacheInputFilter *target = (ApacheInputFilter *)f->ctx;
        return f->r ? target->request_input_filter(f, b, eMode, eBlock, 
                                                   readbytes) 
            : target->connection_input_filter(f, b, eMode, eBlock, readbytes);
    }
    
    int cpp_call_output_filter(ap_filter_t *f,
                               apr_bucket_brigade *b)
    {
        ApacheOutputFilter *target = (ApacheOutputFilter *)f->ctx;
        return f->r ? target->request_output_filter(f, b) 
            : target->connection_output_filter(f, b);
    }
    

    int cpp_insert_connection_filters(conn_rec *c, void *csd)
    {
        cpp_server_rec *server_rec = (cpp_server_rec *)
            ap_get_module_config(c->base_server->module_config,
                                 &cplusplus_module);
        char **cur_filter = server_rec->output_filters; 
        while( cur_filter != NULL && *cur_filter != NULL ) { 
            ApacheOutputFilter *filter = (ApacheOutputFilter *)
                apr_hash_get(server_rec->output_filter_hash, *cur_filter, 
                             strlen(*cur_filter));
            if( filter ) {
                ap_add_output_filter(*cur_filter, filter, NULL, c);
            }
            cur_filter++;
        } 
        cur_filter = server_rec->input_filters;
        while( cur_filter != NULL && *cur_filter != NULL ) { 
            ApacheInputFilter *filter = (ApacheInputFilter *)
                apr_hash_get(server_rec->input_filter_hash, *cur_filter, 
                             strlen(*cur_filter));
            if( filter ) {
                ap_add_input_filter(*cur_filter, filter, NULL, c);
            }
            cur_filter++; 
        }
        return OK;
    }
    
    void cpp_insert_request_filters(request_rec *r)
    {
        cpp_config_rec *dir_rec = (cpp_config_rec *) 
            ap_get_module_config(r->per_dir_config, 
                                 &cplusplus_module);
        cpp_server_rec *server_rec = (cpp_server_rec *)
            ap_get_module_config(r->server->module_config,
                                 &cplusplus_module);
        
        char **cur_filter = dir_rec->output_filters; 
        while( cur_filter != NULL && *cur_filter != NULL ) { 
            ApacheOutputFilter *filter = (ApacheOutputFilter *)
                apr_hash_get(server_rec->output_filter_hash, *cur_filter, 
                             strlen(*cur_filter));
            if( filter ) {
                ap_add_output_filter(*cur_filter, filter, r, r->connection);
            }
            cur_filter++;
        }
        cur_filter = dir_rec->input_filters;
        while( cur_filter != NULL && *cur_filter != NULL ) {
            ApacheInputFilter *filter = (ApacheInputFilter *)
                apr_hash_get(server_rec->input_filter_hash, *cur_filter,
                             strlen(*cur_filter));
            if( filter ) {
                ap_add_input_filter(*cur_filter, filter,r,r->connection);
            }
            cur_filter++;
        }
    }
}
