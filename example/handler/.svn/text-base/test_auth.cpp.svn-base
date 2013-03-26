#include "test_auth.h"
#include "apr_strings.h"
AuthHandler::AuthHandler(ApacheServerRec *pServer)
:    ApacheHandler(pServer)
{
}

AuthHandler::~AuthHandler()
{
}

int AuthHandler::check_user_id(ApacheRequestRec *pRequest)
{
    char *require_user=0;
    int result = OK;
    const char *sent_pw=0;
    
    result = pRequest->get_basic_auth_pw(&sent_pw);
    
    if( !result ) {
        ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                     "got pw: %s", sent_pw);

        require_user = get_cpp_var(pRequest, "user");
        ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                     "got require_user: %s", require_user);
    
        if( require_user ) {
            const char *sent_user = pRequest->user();
            ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                         "ummmmmm %s..%s!!!!", require_user, sent_user);
            if( !sent_user || strcmp(sent_user, require_user) )
                result = HTTP_UNAUTHORIZED;
        }
    }
    ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                 "result %d!!!!", result);
    return result;
}

int AuthHandler::auth_checker(ApacheRequestRec *pRequest)
{
    ap_log_error(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, 0, NULL,
                 "authz baby");
    return OK;
}

/*=====================INSTANCIATORS==========================*/

ApacheHandler *instanciate_auth(ApacheServerRec *pServer)
{
    return new AuthHandler(pServer);
}

cpp_factory_t test_auth = {
    instanciate_auth,
    NULL,
    NULL,
    NULL,
};
