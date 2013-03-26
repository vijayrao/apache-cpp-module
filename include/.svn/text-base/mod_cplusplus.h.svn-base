#ifndef MOD_CPLUSPLUS_H
#define MOD_CPLUSPLUS_H

#include <httpd.h>
#include <http_config.h>
#include <http_protocol.h>
#include <http_request.h>
#include <http_connection.h>
#include <http_log.h>
#include <apr_strings.h>
#include <apr_hash.h>

#define MAX_HANDLERS 20

typedef struct {
    apr_hash_t *handler_hash;
    apr_hash_t *input_filter_hash;
    apr_hash_t *output_filter_hash;
    apr_hash_t *protocol_hash;
    /** used for child caching the config so the child processes can spawn the handlers **/
    apr_hash_t *handler_path_hash;

    char **protocol_handlers;
    char **input_filters;
    char **output_filters;
    apr_hash_t *var_hash;
} cpp_server_rec;

typedef struct {
    char **handlers;
    char **input_filters;
    char **output_filters;
    apr_hash_t *var_hash;
} cpp_config_rec;

/* module AP_MODULE_DECLARE_DATA cplusplus_module; */

#ifdef __cplusplus
extern "C" {
#endif
    extern AP_MODULE_DECLARE_DATA module cplusplus_module;
    extern int cpp_call_handler(request_rec *r); 
    extern int cpp_call_post_read_request(request_rec *r);
    extern int cpp_call_translate_name(request_rec *r);
    extern int cpp_call_header_parser(request_rec *r);
    extern int cpp_call_access_checker(request_rec *r);
    extern int cpp_call_check_user_id(request_rec *r);
    extern int cpp_call_auth_checker(request_rec *r);
    extern int cpp_call_type_checker(request_rec *r);
    extern int cpp_call_fixups(request_rec *r); 
    extern int cpp_call_logger(request_rec *r);
    extern apr_status_t delete_handler(apr_hash_index_t *data);
    extern void cpp_spawn_handlers(apr_pool_t *p, server_rec *s);
    extern apr_status_t cpp_close_handlers(void *data);
    extern apr_status_t cpp_call_input_filter(ap_filter_t *f,
                                              apr_bucket_brigade *b,
                                              ap_input_mode_t eMode,
                                              apr_read_type_e eBlock,
                                              apr_off_t readbytes);
    
    extern int cpp_call_output_filter(ap_filter_t *f,
                                              apr_bucket_brigade *b);
    
    extern void cpp_insert_request_filters(request_rec *r);

    extern int cpp_insert_connection_filters(conn_rec *c, void *csd);
    extern char *load_cpp_module(apr_pool_t *pool, server_rec*, cpp_server_rec *server_rec, apr_hash_index_t *next); 
    extern int cpp_call_process_connection(conn_rec *c);
#ifdef __cplusplus
}
#endif

#endif /* MOD_CPLUSPLUS_H */
