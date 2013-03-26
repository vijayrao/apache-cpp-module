#ifndef __REQUEST_ENV_H__
#define __REQUEST_ENV_H__

#ifdef WIN32
#ifdef EXPORT_MODCPP
#define MODCPP_API __declspec(dllexport)
#else
#define MODCPP_API __declspec(dllimport)
#endif /* EXPORT_MODCPP */
#else
#define MODCPP_API
#endif /* WIN32 */

#include "apache_filters.h"
#include <functional>
#include <vector>
#include <string>

#include "apache_output_buffer.h"
#include "env_value.h"

class env_value;

class MODCPP_API request_env : public std::ostream {
 public:
    
    request_env(  request_rec *r,bool buffer = false );
    
    /* dump the values in ( hopefully ) nicely formatted html */
    void dump();
    
    /* is request a post? */
    bool s_post();
    
    /* returns number of bytes written to client, -1 on failure */
    int flush();
    
    /* set the content type. Defaults to text/html if not set */
    bool set_content_type(const std::string& type);
    
    void set_buffered( bool val );
    bool is_buffered();
    void clear_buffer();
    void flush_buffer();
    bool is_post();
    bool is_get();
    
    env_value* operator[]( const std::string& name );

    /* representing multipart/form-data data
     * The transmited file-content could be include a character with code 0.
     * We can not handle this string as a normal c-string. */
    char *form_data;
    int form_data_length;
    std::string boundary;

    /* Wrap call to uri function of request record */
    const char *uri() const { return r_->uri; }
    
    request_rec* raw_record();
    
    ~request_env();
 private:
   
    apache_output_buffer output_buffer_;
    request_rec *r_;
    std::vector<env_value *> env_;

    env_value* search(const std::string& key);
    
    bool truncated_;
    void decode(  char *ch, env_value *pPtr, std::string& pStr );
    apr_size_t len_read_;
    bool is_post_;
    
};


#endif /* __REQUEST_ENV_H__ */
