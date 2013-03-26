#ifndef __APACHE_FILTERS_H__
#define __APACHE_FILTERS_H__

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

class MODCPP_API ApacheInputFilter : public ApacheBase
{
public:
    /**
     * \brief Constructor for your input filter - called when the child thread/process is initiated
     **/
    ApacheInputFilter(ApacheServerRec *pServer) { }

    /**
     * \brief Constructor for your input filter - called when the child thread/process is cleaned up
     **/
    virtual ~ApacheInputFilter() { }

    /**
     * \brief connection_input_filter is called to process data.  This is calleed once per brigade of data
     *
     * \param f the filter context
     * \param b the bucket brigade of data
     * \param eMode input filtering node (e.g. AP_MODE_GETLINE)
     * \param eBlock tells the filter wether or not it should block
     * \param readbytes if the eMode is AP_MODE_READBYTESÊthis tells the module how many bytes
     **/
    virtual apr_status_t connection_input_filter(ap_filter_t *f, 
                                                 apr_bucket_brigade *b,
                                                 ap_input_mode_t eMode, 
                                                 apr_read_type_e eBlock,
                                                 apr_off_t readbytes)
    { return DECLINED; }

    /**
     * \brief request_input_filter is called to process data.  This is calleed once per brigade of data
     * for a request
     *
     * \param f the filter context
     * \param b the bucket brigade of data
     * \param eMode input filtering node (e.g. AP_MODE_GETLINE)
     * \param eBlock tells the filter wether or not it should block
     * \param readbytes if the eMode is AP_MODE_READBYTES this tells the module how many bytes
     **/

    virtual apr_status_t request_input_filter(ap_filter_t *f, 
                                              apr_bucket_brigade *b,
                                              ap_input_mode_t eMode, 
                                              apr_read_type_e eBlock,
                                              apr_off_t readbytes) 
    { return DECLINED; }
};

class MODCPP_API ApacheOutputFilter : public ApacheBase
{
public:
    /**
     * \brief Constructor for your output filter - called when the child thread/process is initiated
     **/
    ApacheOutputFilter(ApacheServerRec *pServer) { }

    /**
     * \brief Destructor for your output filter - called when the child thread/process is cleaned up
     **/
    virtual ~ApacheOutputFilter() { }

    /**
     * \brief connection_output_filter is the method thats called for you to handle data
     *
     * \param f the filter context
     * \param b the brigade to filter
     **/
    virtual int connection_output_filter(ap_filter_t *f, 
                                         apr_bucket_brigade *b)
    { return DECLINED; }
    
    /**
     * \brief request_output_filter is the method thats called for you to handle data
     *
     * \param f the filter context
     * \param b the brigade to filter
     **/
    virtual int request_output_filter(ap_filter_t *f, apr_bucket_brigade *b) 
    { return DECLINED; }
};

#endif /* __APACHE_FILTERS_H__ */
