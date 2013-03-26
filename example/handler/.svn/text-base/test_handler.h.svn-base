#include "apache_filters.h"

class TestHandler : public ApacheHandler
{
private:
    unsigned int mHits;
public:
    TestHandler(ApacheServerRec *pServer);
    ~TestHandler(void);
    int handler(ApacheRequestRec *pRequest);
};
