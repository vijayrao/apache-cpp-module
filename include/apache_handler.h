#ifndef APACHE_HANDLER_H
#define APACHE_HANDLER_H

#ifdef WIN32
#ifdef EXPORT_MODCPP
#define MODCPP_API __declspec(dllexport)
#else
#define MODCPP_API __declspec(dllimport)
#endif /* EXPORT_MODCPP */
#else
#define MODCPP_API
#endif /* WIN32 */

#include "mod_cplusplus.h"
#include "cpp_request.h"

class MODCPP_API ApacheInputFilter;
class MODCPP_API ApacheOutputFilter;
class MODCPP_API ApacheProtocol;

class MODCPP_API ApacheBase
{
protected:
    /**
     * \brief This method allows your module to query the core for a parameter
     *        passed via the PassCPPVar directive in a directory block of the 
     *        httpd.conf file.
     *
     * \param r An ApacheRequestRec argument
     * \param name The name of the parameter you are looking for
     * \return The value of the parameter
     * \sa get_server_var()
     **/
    char *get_cpp_var(ApacheRequestRec *r, const char *name); 
    /**
     * \brief This method allows your module to query the core for a parameter
     *        passed via the PassCPPVar directive in a server block of the 
     *        httpd.conf file.
     *
     * \param r An ApacheRequestRec argument
     * \param name The name of the parameter you are looking for
     * \return The value of the parameter
     * \sa get_cpp_var()
     **/
    char *get_server_var(ApacheServerRec *r, const char *name); 
    
};

class MODCPP_API ApacheHandler : public ApacheBase
{
public:
    /**
     * \brief Default Constructor.  This is called when a child process/thread is created
     **/
    ApacheHandler(ApacheServerRec *server) { }

    /**
     * \brief Default destructor.  This is called when a child process/thread is destroyed
     **/
    virtual ~ApacheHandler(void) { }

    /**
     * \brief The handler method is called whenever a location mapped to your module is hit
     *        during the handler phase of the request cycle.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int handler(ApacheRequestRec *r) {return DECLINED;}

    /**
     * \brief The fixups method is called whenever a location mapped to your module is hit
     *        during the fixups phase of the request cycle.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int fixups(ApacheRequestRec *r) {return DECLINED;}

    /**
     * \brief The post_read_request method is called whenever a location mapped to your module is hit
     *        immediately after the request is created.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int post_read_request(ApacheRequestRec *r) {return DECLINED;}

    /**
     * \brief The translate_name method is called whenever a location mapped to your module is hit
     *        during the name translation phase.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int translate_name(ApacheRequestRec *r) {return DECLINED;}

    /**
     * \brief The header_parser method is called whenever a location mapped to your module is hit
     *        during the  header parsing phase.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int header_parser(ApacheRequestRec *r) {return DECLINED;}

    /**
     * \brief The access_checker method is called whenever a location mapped to your module is hit
     *        during the access phase.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int access_checker(ApacheRequestRec *r) {return DECLINED;}

    /**
     * \brief The check_user_id method is called whenever a location mapped to your module is hit
     *        during the authentication phase.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int check_user_id(ApacheRequestRec *r) {return DECLINED;}

    /**
     * \brief The auth_checker method is called whenever a location mapped to your module is hit
     *        during the authorization phase.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int auth_checker(ApacheRequestRec *r) {return DECLINED;}

    /**
     * \brief The type_checker method is called whenever a location mapped to your module is hit
     *        during the type checking phase.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int type_checker(ApacheRequestRec *r) {return DECLINED;}

    /**
     * \brief The logger method is called whenever a location mapped to your module is hit
     *        during the logging phase.
     *
     * \param r ApacheRequestRec containing the current request
     **/
    virtual int logger(ApacheRequestRec *r) {return DECLINED;}
};

typedef ApacheHandler *(*handler_factory)(ApacheServerRec *);
typedef ApacheInputFilter *(*input_filter_factory)(ApacheServerRec *);
typedef ApacheOutputFilter *(*output_filter_factory)(ApacheServerRec *);
typedef ApacheProtocol *(*protocol_factory)(ApacheServerRec *);

/**
 * \brief cpp_factory_t is the callback structure that allows a module to hook a type of handler.
 **/
typedef struct {
    handler_factory handler_func;
    input_filter_factory input_filter_func;
    output_filter_factory output_filter_func;
    protocol_factory protocol_func;   
} cpp_factory_t;

#endif /* APACHE_HANDLER_H */
