#ifndef __APACHE_OUTPUT_BUFFER__
#define __APACHE_OUTPUT_BUFFER__

#ifdef WIN32
#ifdef EXPORT_MODCPP
#define MODCPP_API __declspec(dllexport)
#else
#define MODCPP_API __declspec(dllimport)
#endif /* EXPORT_MODCPP */
#else
#define MODCPP_API
#endif /* WIN32 */

#include "httpd.h"
#include "http_request.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"

#include <iostream>
#include <string>

class MODCPP_API apache_output_buffer : public std::streambuf {
 public: 
    apache_output_buffer( request_rec* r, bool buffer = false );
    
    ~apache_output_buffer();
    
    /* if buffering output, send buffered output to browser immediatly */
    int flush();
    
    /* if beffering output, clear buffer */
    void clear();
    
    /* set whether or not to buffer output */
    void buffer ( bool val ); 
    
 	/* is output being buffered? */
    bool buffer();
    
    /* set what content type should be sent
     * to browser.  Returns true on success, false
     * if we've already sent some content, and it's to
     * late to resend. 
     */
    bool set_content_type(const std::string& type);
    
 protected:
    int overflow(int c);
    
 private:
    inline void signal_sending();
    inline void send_http_header();
    request_rec *r_;
    std::string memory_;
    bool buffer_;
    bool output_anything_;
    bool update_content_type_;
    std::string content_type_;
};

#endif /* __APACHE_OUTPUT_BUFFER__ */
