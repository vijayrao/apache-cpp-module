#include "apache_filters.h"

class TestInputFilter : public ApacheInputFilter
{
private:
    char *reverse_args(apr_pool_t *pool, const char *args);
    char *pszToLower;

public:
  
    TestInputFilter(ApacheServerRec *pServer);
    ~TestInputFilter(void);

    apr_status_t connection_input_filter(ap_filter_t *f, apr_bucket_brigade *b,
                                         ap_input_mode_t eMode, 
                                         apr_read_type_e eBlock,
                                         apr_off_t readbytes);

};
