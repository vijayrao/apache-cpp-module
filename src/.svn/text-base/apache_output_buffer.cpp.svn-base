#define EXPORT_MODCPP

#ifndef NO_STDCPP
#include "apache_output_buffer.h"
#include "http_log.h"

apache_output_buffer::apache_output_buffer( request_rec* r, bool buffer) : 
    r_(r),
    buffer_(buffer) 
{
    output_anything_=false;
    update_content_type_ = false;
    content_type_="text/html";
}

apache_output_buffer::~apache_output_buffer() {
    
}

int
apache_output_buffer::flush() {
    this->signal_sending();
    
    int ret_val;
    ret_val = ap_rputs(memory_.c_str(), r_ );
    if ( -1 != ret_val ) {
        memory_ = "";
    }
    
    return ret_val;
}

void
apache_output_buffer::clear(){
    memory_ = "";
}

void
apache_output_buffer::buffer ( bool val ){
    if ( buffer_ && ! val )
        this->flush();
    buffer_ = val;
}

bool
apache_output_buffer::buffer(){
    return buffer_;
}

int
apache_output_buffer::overflow(int c) {
    /* believe it or not, it's quicker to just ouput every char here,
     * rather than buffer and output on class destruct
     */
    if (buffer_) {
        memory_ += static_cast<char>(c);
    } else {
        this->signal_sending();
        ap_rputc( static_cast<char>(c),r_ );
    }
    return c;
}

void
apache_output_buffer::signal_sending(){
    if ( ! output_anything_ ){
        this->send_http_header();
        output_anything_=true;
    }
}

void
apache_output_buffer::send_http_header(){
  if( update_content_type_ ) {
    ap_set_content_type(r_,content_type_.c_str() );
  }
}

bool
apache_output_buffer::set_content_type(const std::string& type){
    content_type_=type;
    update_content_type_ = true;
    return ! output_anything_;
}
#endif
