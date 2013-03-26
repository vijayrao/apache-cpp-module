/* ====================================================================
 * Copyright (c) 2001 John K. Sterling.  All rights reserved.
 * See the file COPYING for information on usage and redistribution.
 * ====================================================================
 */

#include "mod_cplusplus.h"

#define PLUS_INHERIT(parent, child, field) \
    ((child)->field? (child)->field : (parent)->field)

static void register_hooks(apr_pool_t * p)
{
    ap_hook_handler(cpp_call_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_access_checker(cpp_call_access_checker, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_check_user_id(cpp_call_check_user_id, NULL, NULL, APR_HOOK_FIRST);
    ap_hook_auth_checker(cpp_call_auth_checker, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_type_checker(cpp_call_type_checker, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_fixups(cpp_call_fixups, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_log_transaction(cpp_call_logger, NULL, NULL, APR_HOOK_MIDDLE);    
    ap_hook_pre_connection(cpp_insert_connection_filters, NULL, NULL, APR_HOOK_FIRST);
    ap_hook_insert_filter(cpp_insert_request_filters, NULL, NULL, APR_HOOK_MIDDLE);    
    ap_hook_process_connection(cpp_call_process_connection, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(cpp_spawn_handlers, NULL, NULL, APR_HOOK_FIRST);
}

static void *merge_cpp_config(apr_pool_t *pool, void *base, void *overrides)
{
    cpp_config_rec *new_config = (cpp_config_rec *)
        apr_pcalloc(pool, sizeof(*new_config));
    cpp_config_rec *parent = base;
    cpp_config_rec *child = overrides;
    new_config->var_hash = PLUS_INHERIT(parent, child, var_hash);
    new_config->handlers = PLUS_INHERIT(parent, child, handlers);
    new_config->input_filters = PLUS_INHERIT(parent, child, input_filters);
    new_config->output_filters = PLUS_INHERIT(parent, child, output_filters);
    return new_config;
}

static void *create_cpp_config(apr_pool_t *p, char *dummy)
{
    cpp_config_rec *new_config = (cpp_config_rec *)
        apr_pcalloc(p, sizeof(*new_config));
    new_config->var_hash = apr_hash_make(p);
    new_config->handlers = apr_pcalloc(p, sizeof(char *) * MAX_HANDLERS);
    new_config->input_filters = apr_pcalloc(p, sizeof(char *) * MAX_HANDLERS);
    new_config->output_filters = apr_pcalloc(p, sizeof(char *) * MAX_HANDLERS);
    return new_config;
}

static void *create_cpp_server_config(apr_pool_t *p, server_rec *s)
{
    cpp_server_rec *new_config = (cpp_server_rec *)
        apr_pcalloc(p, sizeof(*new_config));
    new_config->handler_hash = apr_hash_make(p);
    new_config->handler_path_hash = apr_hash_make(p);
    new_config->var_hash = apr_hash_make(p);
    new_config->input_filter_hash = apr_hash_make(p);
    new_config->output_filter_hash = apr_hash_make(p);
    new_config->protocol_hash = apr_hash_make(p);
      
    new_config->input_filters = apr_pcalloc(p, sizeof(char *) * MAX_HANDLERS);
    new_config->output_filters = apr_pcalloc(p, sizeof(char *) * MAX_HANDLERS);
    new_config->protocol_handlers = apr_pcalloc(p, sizeof(char *) * 
                                                MAX_HANDLERS);
    return new_config;
}

apr_status_t cpp_close_handlers(void *data) {
    server_rec *s = data;
    cpp_server_rec *server_rec = (cpp_server_rec *)
        ap_get_module_config(s->module_config,
                             &cplusplus_module);
    apr_hash_index_t *next = apr_hash_first(s->process->pool, server_rec->handler_hash);
    while( next != NULL ) {
        delete_handler(next);
	next = apr_hash_next(next);
    }
    return APR_SUCCESS;
}

static const char *load_handler(apr_pool_t *pool, server_rec *server, cpp_server_rec *server_rec, apr_hash_index_t *next) {
    return load_cpp_module(pool, server, server_rec, next);
}

const char *load_handlers(apr_pool_t *pool, server_rec *server, cpp_server_rec *server_rec) {
    apr_hash_index_t *next = apr_hash_first(pool, server_rec->handler_path_hash);
    while( next != NULL ) {
        load_handler(pool, server, server_rec, next);
	next = apr_hash_next(next);
    }
    return NULL;
}


void cpp_spawn_handlers(apr_pool_t *p, server_rec *s) {
    cpp_server_rec *server_rec = (cpp_server_rec *)
        ap_get_module_config(s->module_config,
                             &cplusplus_module);
    load_handlers(p, s, server_rec);
    apr_pool_cleanup_register(p, s, cpp_close_handlers, cpp_close_handlers);
}

static const char *add_handler(cmd_parms *cmd, void *config, 
                               const char *name,
                               const char *path_to_so)
{
    cpp_server_rec *server_rec = (cpp_server_rec *)
        ap_get_module_config(cmd->server->module_config,
                             &cplusplus_module);
    apr_hash_set(server_rec->handler_path_hash, name, strlen(name), path_to_so);
    return NULL;
}

static const char *pass_server_var(cmd_parms *cmd, void *config,
                                   const char *name,
                                   const char *value)
{
    cpp_server_rec *server_rec = (cpp_server_rec *)
        ap_get_module_config(cmd->server->module_config,
                             &cplusplus_module);
    apr_hash_set(server_rec->var_hash, name, strlen(name), value);
    return NULL;
}

static const char *pass_var(cmd_parms *cmd, void *config, 
                               const char *name,
                               const char *value)
{
    cpp_config_rec *cmd_rec = (cpp_config_rec *)config;
    apr_hash_set(cmd_rec->var_hash, name, strlen(name), value);
    return NULL;
}

static const char *add_dir_handler(cmd_parms *cmd, void *config, 
                                   const char *name)
{
    cpp_config_rec *cmd_rec = (cpp_config_rec *)config;
    char **cur_ptr = cmd_rec->handlers;
    int i = 0;
    while(*cur_ptr != NULL) {
        cur_ptr++;
        i++;
        if( i >= MAX_HANDLERS ) {
            return "Too Many Handlers!! Couldn't add handler";
        }
    }
    *cur_ptr = apr_pstrdup(cmd->pool, name);
    return NULL;
}

static const char *add_protocol_handler(cmd_parms *cmd, void *config, 
                                        const char *name)
{
    cpp_server_rec *server_rec = (cpp_server_rec *)
        ap_get_module_config(cmd->server->module_config,
                             &cplusplus_module);
    char **cur_ptr = server_rec->protocol_handlers;
    
    int i = 0;
    while(*cur_ptr != NULL) {
        cur_ptr++;
        i++;
        if( i >= MAX_HANDLERS ) {
            return "Too Many Handlers!! Couldn't add protocol handler";
        }
    }
    *cur_ptr = apr_pstrdup(cmd->pool, name) ;
    return NULL;
}

static const char *add_conn_input_filter(cmd_parms *cmd, void *config, 
                                         const char *name)
{
    cpp_server_rec *server_rec = (cpp_server_rec *)
        ap_get_module_config(cmd->server->module_config,
                             &cplusplus_module);
    char **cur_ptr = server_rec->input_filters;
    
    int i = 0;
    while(*cur_ptr != NULL) {
        cur_ptr++;
        i++;
        if( i >= MAX_HANDLERS ) {
            return "Too Many Handlers!! Couldn't add input filter";
        }
    }
    *cur_ptr = apr_pstrdup(cmd->pool, name);
    ap_register_input_filter(name, 
                             cpp_call_input_filter, 
                             NULL,
                             AP_FTYPE_CONNECTION);
    return NULL;
}

static const char *add_conn_output_filter(cmd_parms *cmd, void *config, 
                                          const char *name)
{
    cpp_server_rec *server_rec = (cpp_server_rec *)
        ap_get_module_config(cmd->server->module_config,
                             &cplusplus_module);
    char **cur_ptr = server_rec->output_filters;
    int i = 0;
    while(*cur_ptr != NULL) {
        cur_ptr++;
        i++;
        if( i >= MAX_HANDLERS ) {
            return "Too Many Handlers!! Couldn't add output filter";
        }
    }
    *cur_ptr = apr_pstrdup(cmd->pool, name);
    ap_register_output_filter(name, 
                              cpp_call_output_filter, 
                              NULL,
                              AP_FTYPE_CONNECTION);
    return NULL; 
}
                               
static const char *add_dir_input_filter(cmd_parms *cmd, void *config, 
                                        const char *name)
{
    cpp_config_rec *cmd_rec = (cpp_config_rec *)config;
    char **cur_ptr = cmd_rec->input_filters;
    
    int i = 0;
    while(*cur_ptr != NULL) {
        cur_ptr++;
        i++;
        if( i >= MAX_HANDLERS ) {
            return "Too Many Handlers!! Couldn't add input filter";
        }
    }
    *cur_ptr = apr_pstrdup(cmd->pool, name);
    ap_register_input_filter(name, 
                             cpp_call_input_filter, 
                             NULL,
                             AP_FTYPE_RESOURCE);
    return NULL;
}

static const char *add_dir_output_filter(cmd_parms *cmd, void *config, 
                                         const char *name)
{
    cpp_config_rec *cmd_rec = (cpp_config_rec *)config;
    char **cur_ptr = cmd_rec->output_filters;
    int i = 0;
    while(*cur_ptr != NULL) {
        cur_ptr++;
        i++;
        if( i >= MAX_HANDLERS ) {
            return "Too Many Handlers!! Couldn't add output filter";
        }
    }
    *cur_ptr = apr_pstrdup(cmd->pool, name);
    ap_register_output_filter(name, 
                              cpp_call_output_filter, 
                              NULL,
                              AP_FTYPE_RESOURCE);
    return NULL;
}
                               
static const command_rec cpp_cmds[] =
{ 
    AP_INIT_TAKE2("LoadCPPHandler", add_handler, NULL, RSRC_CONF,
                  "cpp initializer"),
    AP_INIT_TAKE1("AddCPPHandler", add_dir_handler, NULL, OR_FILEINFO,
                  "add handler"),
    AP_INIT_TAKE1("AddCPPInputFilter", add_dir_input_filter, NULL, OR_FILEINFO,
                  "add input filter"),
    AP_INIT_TAKE1("AddCPPOutputFilter", add_dir_output_filter, NULL, 
                  OR_FILEINFO,
                  "add output filter"),
    AP_INIT_TAKE1("AddCPPConnectionInputFilter", add_conn_input_filter, 
                  NULL, RSRC_CONF,
                  "add connection_input filter"),
    AP_INIT_TAKE1("AddCPPConnectionOutputFilter", add_conn_output_filter, 
                  NULL, 
                  RSRC_CONF,
                  "add connection_output filter"),
    AP_INIT_TAKE1("AddCPPProtocol", add_protocol_handler, NULL, 
                  RSRC_CONF,
                  "add protocol handler"),
    AP_INIT_TAKE2("PassCPPVar", pass_var, NULL, OR_FILEINFO, 
                  "pass var to cpp handlers via request config (per dir)"),
    AP_INIT_TAKE2("PassCPPServerVar", pass_server_var, NULL, RSRC_CONF,
                  "pass var to cpp handlers via server config"),
    {NULL}
};

module AP_MODULE_DECLARE_DATA cplusplus_module =
{
    STANDARD20_MODULE_STUFF, 
    create_cpp_config,             /* dir config creater */ 
    merge_cpp_config,              /* dir merger --- default is to override */
    create_cpp_server_config,      /* server config */ 
    NULL,                          /* merge server configs */ 
    cpp_cmds,       	           /* command apr_table_t */
    register_hooks                 /* register hooks */  
};


