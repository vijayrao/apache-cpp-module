#define EXPORT_MODCPP

#include "cpp_server.h"

ApacheServerRec::ApacheServerRec(server_rec *s)
{
    mServer = s;
    mNext = s->next ? new ApacheServerRec(s->next) : NULL;
}

ApacheServerRec::~ApacheServerRec()
{
    mServer = NULL;
    delete mNext;
}

void *ApacheServerRec::get_module_config(module *m)
{
    return ap_get_module_config(mServer->module_config, m);
}
