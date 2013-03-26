#define EXPORT_MODCPP
#ifndef NO_STDCPP

#include "request_env.h"

#define ENV_READ_LEN 1024

void
request_env::set_buffered( bool val ){
    output_buffer_.buffer(val);
}

void
request_env::clear_buffer(){
    output_buffer_.clear();
}

bool
request_env::is_buffered(){
    return output_buffer_.buffer();
}

void
request_env::flush_buffer(){
    output_buffer_.flush();
}

request_env::request_env( request_rec *r, bool buffer ) :
    /* seems silly/dangerous to to do this, as output_buffer 
     * isn't defined yet, but is needed to keep gcc 3.x happy
     */
    std::ostream( &output_buffer_ ) ,
         output_buffer_( r,buffer ) , r_(r)
{
    
    this->rdbuf( &output_buffer_ );
    
    std::string cur_env;
    env_value *pPtr=0;
    
    len_read_=0;
    form_data = NULL;
    form_data_length = 0;
    
    ap_setup_client_block(r_, REQUEST_CHUNKED_ERROR);
    
    if( ( r_->method_number == M_POST ) || ( r_->method_number == M_PUT ) ) {
        is_post_ = true;
        char buf[524288], *sbuf = NULL;
	int all_read = 0;

	const char *content_length = apr_table_get(r_->headers_in,"Content-Length");

	/* Reading without knowing about the length to read (slow). */
	if ( !content_length ) {
		while( (len_read_ =  ap_get_client_block(r_, buf, sizeof(buf)-1) ) > 0) {
			char *tmp = new char[all_read+sizeof(buf)];
			if ( sbuf ) {
				memcpy(tmp,sbuf,all_read);
				delete sbuf;
				sbuf = NULL;
			}
			memcpy(tmp+all_read,buf,len_read_);
			all_read += len_read_;
			tmp[all_read] = 0;
			sbuf = tmp;
		}
	}

	/* First allocing whole memory, than read the whole content (fast). */
	else {
		int clen = atoi(content_length);
		sbuf = new char[clen+1];
		while ( (len_read_ =  ap_get_client_block(r_, sbuf+all_read, clen)) >0 ) {
			all_read += len_read_;
		}
		sbuf[all_read] = 0;
	}

	/* Resolve content type from input header. */
	const char *content_type = apr_table_get(r_->headers_in,"Content-Type");
	std::string sct, bnd;

	/* If content type is defined. */
	if ( content_type ) {
		sct = content_type;
		int p;
#define BDEF "; boundary="
		if ( (p=sct.find(BDEF)) > -1 ) {
			if ( sct.substr(0,p) == "multipart/form-data" ) {
				bnd = sct.substr(p+strlen(BDEF),sct.length()-p-strlen(BDEF));
				sct.erase(sct.begin()+p,sct.end());
			}
		}
	}
	else sct = "text/plain";

	/* Select parsing function by content type. */
	if ( sct == "application/x-www-form-urlencoded" ) {
	        this->decode( sbuf,pPtr,cur_env );
		if ( sbuf ) delete sbuf;
	}
	else if ( sct == "multipart/form-data" ) {
		boundary = bnd;
		form_data = sbuf;
		form_data_length = all_read;
	}
	else {
		std::cerr << "Undefined content-type in input header: " << sct << std::endl;
		if ( sbuf ) delete sbuf;
	}

    } else {
        if (r->args)
            this->decode( r->args,pPtr,cur_env );
    }
}

void
request_env::decode( char *ch, env_value *pEnv, std::string& pStr ){
    
    /* basic steps in alogorith taken from: 
     * http://www.papillion.ne.us/~sthomas/cgi_in_c.html
     * I like this better than the examples given on chap 11 of the eagle book, 
     * as using that method
     * requires parsing the string more than once.
     */
    size_t ch_len = strlen(ch);
    
    for (size_t i = 0 ; i < ch_len; i++ ){
    
        switch ( ch[i] ) {
            
        /* test for end of string */
        case '\0':
            i = ch_len;
            break;
            
            /* have we reached the end of the curent name? */
        case '=':
            if ( ! pStr.empty() ) {
                pEnv = this->search(pStr);
                if ( ! pEnv ) {
                    pEnv = new env_value( pStr );
                    env_.push_back(pEnv);
                }
                pStr = "";
            }
            break;
            
            /* have we reached the end of the value, 
             * and are switching to a new name? 
             */
        case '&':
            if ( ( ! pStr.empty() ) && pEnv) {
                pEnv->add_value( pStr );
                pStr = "";
            }
            break;
            /* Convert all + chars to space chars */
        case '+':
            pStr += " ";
            break;
            
            /* Convert all %xy hex codes into ASCII chars */
        case '%':
            if ( i < ch_len+2 ) {
                char hexstr[2];
                i++;
                hexstr[0] = ch[i];
                i++;
                hexstr[1] = ch[i];
                
                /* Convert the hex to ASCII */
                pStr += static_cast<char>( strtol(hexstr, NULL, 16) );
            }
            break;
            
            /* Make an exact copy of anything else */
        default:
            pStr +=  ch[i];
            break;
        }
    }
    if ( ( ! pStr.empty() ) && pEnv){
        pEnv->add_value( pStr );
    }
}

request_rec*
request_env::raw_record(){
    return r_;
}

request_env::~request_env(){
    output_buffer_.flush();
}

bool
request_env::is_post(){
    return  is_post_;
}

bool
request_env::is_get(){
    return  ! is_post_;
}

int
request_env::flush() {
    return output_buffer_.flush();
}

env_value*
request_env::search(const std::string& key){
	std::vector<env_value *>::const_iterator it, _end = env_.end();
	for (it = env_.begin(); it != _end; it++) {
		 if ( (*it)->key() == key ) {
			 return *it;
		 }
	}
	return NULL;
}

void
request_env::dump(){
    
    (*this) << "I recieved: " <<  env_.size() << "  elements\n";
}

bool
request_env::set_content_type(const std::string& type){
    return output_buffer_.set_content_type(type);
}

env_value*
request_env::operator[](const std::string& name ){
    return this->search( name );
}
#endif
