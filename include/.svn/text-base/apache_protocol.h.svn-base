#ifndef APACHE_PROTOCOL_H
#define APACHE_PROTOCOL_H

#ifdef WIN32
#ifdef EXPORT_MODCPP
#define MODCPP_API __declspec(dllexport)
#else
#define MODCPP_API __declspec(dllimport)
#endif /* EXPORT_MODCPP */
#else
#define MODCPP_API
#endif /* WIN32 */

#include "apache_handler.h"

class MODCPP_API ApacheProtocol
{
public:
    ApacheProtocol(ApacheServerRec *pServer) { }
    virtual ~ApacheProtocol() { }
    virtual int process_connection(conn_rec *c) { return DECLINED; }
};

#endif /* APACHE_PROTOCOL_H */
