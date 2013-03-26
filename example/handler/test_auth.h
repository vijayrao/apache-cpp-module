#include "apache_filters.h"

class AuthHandler : public ApacheHandler
{
public:
    AuthHandler(ApacheServerRec *pServer);
    ~AuthHandler(void);
    int check_user_id(ApacheRequestRec *pRequest);
    int auth_checker(ApacheRequestRec *pRequest);
};
