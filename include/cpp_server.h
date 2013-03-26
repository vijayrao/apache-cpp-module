#ifndef CPP_SERVER_H
#define CPP_SERVER_H

#ifdef WIN32
#ifdef EXPORT_MODCPP
#define MODCPP_API __declspec(dllexport)
#else
#define MODCPP_API __declspec(dllimport)
#endif /* EXPORT_MODCPP */
#else
#define MODCPP_API
#endif /* WIN32 */

#include <httpd.h>
#include <http_config.h>

class MODCPP_API ApacheServerRec
{
private:
    server_rec *mServer;
    ApacheServerRec *mNext;
    
public:
    ApacheServerRec(server_rec *s);
    ~ApacheServerRec();
    
    process_rec *process() { return mServer->process; }
    ApacheServerRec *next() { return mNext; }
    const char *defn_name() { return mServer->defn_name; }
    unsigned defn_line_number() { return mServer->defn_line_number; }
    
    char *server_admin() { return mServer->server_admin; }
    char *server_hostname() { return mServer->server_hostname; }
    apr_port_t port() { return mServer->port; }
    
    char *error_fname() { return mServer->error_fname; }
    apr_file_t *error_log() { return mServer->error_log; }
    int loglevel() { return mServer->loglevel; }
    int is_virtual() { return mServer->is_virtual; }
    
    ap_conf_vector_t *lookup_defaults() { return mServer->lookup_defaults; }
    server_addr_rec *addrs() { return mServer->addrs; }
    
    apr_interval_time_t timeout() { return mServer->timeout; }
    apr_interval_time_t keep_alive_timeout() { return mServer->keep_alive_timeout; }
    int keep_alive_max() { return mServer->keep_alive_max; }
    int keep_alive() { return mServer->keep_alive; }
    
    const char *path() { return mServer->path; }
    int pathlen() { return mServer->pathlen; }
    
    apr_array_header_t *names() { return mServer->names; }
    apr_array_header_t *wild_names() { return mServer->wild_names; }
    
    int limit_req_line() { return mServer->limit_req_line; }
    int limit_req_fieldsize() { return mServer->limit_req_fieldsize; }
    int limit_req_fields() { return mServer->limit_req_fields; }
    
    server_rec *get_server_rec() { return mServer; }
  
    void *get_module_config(module *m);
};

#endif /* CPP_SERVER_H */
