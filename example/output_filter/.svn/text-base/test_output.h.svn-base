#include "apache_filters.h"

class TestOutputFilter : public ApacheOutputFilter
{
public:
    TestOutputFilter(ApacheServerRec *pServer);
    ~TestOutputFilter(void);
    apr_status_t request_output_filter(ap_filter_t *f, apr_bucket_brigade *b);
    int connection_output_filter(ap_filter_t *f, 
                                 apr_bucket_brigade *b);
    
    
private:
};

