#define EXPORT_MODCPP

#include "apache_protocol.h"

extern "C" {
    apr_status_t cpp_call_process_connection(conn_rec *c)
    {
        int result = DECLINED; 
        ApacheServerRec *pServer = new ApacheServerRec(c->base_server);
        ApacheProtocol *pProtocol = NULL;
  
        cpp_server_rec *server_rec = (cpp_server_rec *)
            pServer->get_module_config(&cplusplus_module);

        char **cur_handler = server_rec->protocol_handlers; 
        while( cur_handler != NULL && *cur_handler != NULL ) { 
            pProtocol = (ApacheProtocol *)
                apr_hash_get(server_rec->protocol_hash, 
                             *cur_handler, 
                             strlen(*cur_handler)); 
            if( pProtocol ) {
                result = pProtocol->process_connection(c); 
            }
            if( result != DECLINED ) { 
                break; 
            } 
            cur_handler++; 
        } 
        delete pServer; 
        return result;    
    }
}
