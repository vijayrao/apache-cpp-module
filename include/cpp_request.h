#ifndef CPP_REQUEST_H
#define CPP_REQUEST_H

#ifdef WIN32
#pragma warning( disable : 4275 4273 4786 )
#ifdef EXPORT_MODCPP
#define MODCPP_API __declspec(dllexport)
#else
#define MODCPP_API __declspec(dllimport)
#endif /* EXPORT_MODCPP */
#else
#define MODCPP_API
#endif /* WIN32 */

#include <httpd.h>
#include <http_request.h>
#include <http_config.h>
#include <http_core.h>
#include <http_protocol.h>
#include <apr_strings.h>
#include <string>   // for std::string
#include "cpp_server.h"

using std::string;
class MODCPP_API ApacheRequestRec
{
protected:
	request_rec *mRequest;
    ApacheRequestRec *mPrev;
    ApacheRequestRec *mNext;
    ApacheRequestRec *mMain;
    ApacheServerRec *mServer;
    /** utility functions for formatting **/
    string istring(int value, const char* format = "%d") const;
    string mstring(const char *cp) const;

public:
	ApacheRequestRec(request_rec *r, ApacheRequestRec *pPrev = NULL, 
                     ApacheRequestRec *pNext = NULL);
    ~ApacheRequestRec();
    
    /** \return The pool associated with the request **/
    inline    apr_pool_t *pool() { return mRequest->pool; }
    /** \return The connection associated with the request **/
    inline    conn_rec *connection() { return mRequest->connection; }
    
    /** \return The server associated with the request **/
    inline    ApacheServerRec *server() { return mServer; }

    /** \return Pointer to the redirected request if this is an external redirect **/
    inline    ApacheRequestRec *next() { return mNext; }

    /** \return Pointer to the previous request if this is an internal redirect **/
    inline    ApacheRequestRec *prev() { return mPrev; }

    /** \return Pointer to the main request if this is a sub-request **/
    inline    ApacheRequestRec *main() { return mMain; }

    /** \return the first line of actual http request content **/
    inline    const char *the_request() const { return mRequest->the_request; }

    /** \return HTTP/0.9, "simple" request (e.g. GET /foo w/no headers) **/
    inline    int assbackwards() const { return mRequest->assbackwards; }

    /** \return A proxy request (calculated during post_read_request/translate_name) possible values PROXYREQ_NONE, PROXYREQ_PROXY, PROXYREQ_REVERSE, PROXYREQ_RESPONSE **/
    inline    int proxyreq() const { return mRequest->proxyreq; }
    
    /** \return HEAD request, as opposed to GET **/
    inline    int header_only() const { return mRequest->header_only; }

    /** \return Protocol string, as given to us, or HTTP/0.9 **/
    inline    const char *protocol() const {return mRequest->protocol; }

    /** \return Protocol version number of protocol; 1.1 = 1001 **/
    inline    int proto_num() const { return mRequest->proto_num; }

    /** \return Host, as set by full URI or Host: **/
    inline    const char *hostname() const { return mRequest->hostname; }

    /** \return Time when the request started **/
    inline    apr_time_t request_time() const { return mRequest->request_time; }

    /** \return Time when the request started **/
    inline    const char *status_line() const { return mRequest->status_line; }

    /** \return Status line **/
    inline    int status() const {return mRequest->status; }

    /** \return Request method (eg. GET, HEAD, POST, etc.) **/
    inline    const char *method() const { return mRequest->method; }

    /** \return M_GET, M_POST, etc. **/
    inline    int method_number() const { return mRequest->method_number; }

    /** 
     * \brief 'allowed' is a bitvector of the allowed methods.
     *
     * A handler must ensure that the request method is one that it is capable of handling. 
     * Generally modules should DECLINE any request methods they do not handle. 
     * Prior to aborting the handler like this the handler should set r->allowed 
     * to the list of methods that it is willing to handle. 
     * This bitvector is used to construct the "Allow:" header required for OPTIONS requests, 
     * and HTTP_METHOD_NOT_ALLOWED and HTTP_NOT_IMPLEMENTED status codes.
     *
     * Since the default_handler deals with OPTIONS, all modules can usually decline to deal with OPTIONS. 
     * TRACE is always allowed, modules don't need to set it explicitly.
     *
     * Since the default_handler will always handle a GET, a module which does *not* implement GET 
     * should probably return HTTP_METHOD_NOT_ALLOWED. Unfortunately this means that a Script GET 
     * handler can't be installed by mod_actions.
     *
     * \return A bit vector of allowed methods
     **/
    inline    apr_int64_t allowed() const { return mRequest->allowed; }

    /** \return Array of extension methods **/
    inline    const apr_array_header_t *allowed_xmethods() const {return mRequest->allowed_xmethods; }

    /** \return List of allowed methods **/
    inline    const ap_method_list_t *allowed_methods() const {return mRequest->allowed_methods; }

    /** \return byte count in stream is for body **/
    inline    apr_off_t sent_bodyct() const {return mRequest->sent_bodyct; }

    /** \return byte count of the body **/
    inline    apr_off_t bytes_sent() const { return mRequest->bytes_sent; }

    /** \return Last modified time of the requested resource **/
    inline    apr_time_t mtime() const { return mRequest->mtime; } 

    /** \return sending chunked transfer-coding **/
    inline    int chunked() const { return mRequest->chunked; }

    /** \return The HTTP range header value **/
    inline    const char *range() const {return mRequest->range; }

    /** \return The so-called 'real' content length **/
    inline    apr_off_t clength() const {return mRequest->clength; }

    /** \return Remaining bytes to be read from the request **/
    inline    apr_off_t remaining() const {return mRequest->remaining; }

    /** \return Number of bytes already read from the request **/
    apr_off_t read_length() const { return mRequest->read_length; }

    /** \return Method for reading the request body (e.g. REQUEST_CHUNKED_ERROR) **/
    inline    int read_body() const { return mRequest->read_body; }

    /** \return Reading chunked encoding **/
    inline    int read_chunked() const { return mRequest->read_chunked; }
    
    /** \return MIME headers from the request **/
    inline    apr_table_t *headers_in() const { return mRequest->headers_in; }

    /** \return MIME headers from the response **/
    inline    apr_table_t *headers_out() const { return mRequest->headers_out; }

    /** \return MIME headers from the response printed even on error **/
    inline    apr_table_t *err_headers_out()  const { return mRequest->err_headers_out; }
    
    /** \return An array of environment variables passed to the subprocess **/
    inline    apr_table_t *subprocess_env() const { return mRequest->subprocess_env; }
    
    /** \return Notes to pass from one module to another **/
    inline    apr_table_t *notes() const { return mRequest->notes; }
    
    /** \return The content type of the request **/
    const char *content_type(char *type = NULL) const 
    { return type ? (mRequest->content_type = 
                     apr_pstrdup(mRequest->pool, type)) : 
        mRequest->content_type; }

    /** \return the name of the handler assigned this request **/
    inline    const char *handler() { return mRequest->handler; }

    /** \return How to encode the data **/
    inline    const char *content_encoding() { return mRequest->content_encoding; }

    /** \return Whether the request body will be discarded **/
    inline    int discard_request_body() { 
        return ap_discard_request_body(mRequest); 
    }
    
    /** \return Send blocking data to the client **/
    inline    int get_client_block(char *buf, apr_size_t bufsiz) {
        return ap_get_client_block(mRequest, buf, bufsiz);
    }
        
    /** \return Array of strings representing content languages **/
    inline    const apr_array_header_t *content_languages() const
    { return mRequest->content_languages; }
   
    /** \return variant list validator (if negotiated) **/
    inline    const char *vlist_validator() const { return mRequest->vlist_validator; }

    /** \return If an authentication check was made, this gets set to the user name. */
    inline    const char *user() const { return mRequest->user; }

    /** \return If an authentication check was made, this gets set to the auth type. */
    inline    const char *ap_auth_type() const { return mRequest->ap_auth_type; }

    /** \return The URI without any parsing performed */
    inline    const char *unparsed_uri() const { return mRequest->unparsed_uri; }

    /** \return Just the path portion of the URI */
    inline    const char *uri() const { return mRequest->uri; }

    /** \return The filename on disk corresponding to this response */
    inline    const char *filename() const { return mRequest->filename; }

    /** \return The PATH_INFO extracted from this request */
    inline    const char *path_info() const { return mRequest->path_info; }

    /** \return The QUERY_ARGS extracted from this request */
    inline    const char *args() const { return mRequest->args; }
    
    /** \return true if this response can not be cached */
    int no_cache() const { return mRequest->no_cache; }

    /** \return true if there is no local copy of this response */
    inline    int no_local_copy() const { return mRequest->no_local_copy; }
    
    /** \return finfo.protection (st_mode) set to zero if no such file */
    inline    apr_finfo_t finfo() const { return mRequest->finfo; }

    /** \return a struct containing the components of URI */
    inline    apr_uri_t parsed_uri() const { return mRequest->parsed_uri; }
    
    /** 
     * \brief pass in a module and this method will returrn you its per-dir config 
     * \param m a pointer to the static module definition
     * \return a void pointer to the config structure that was set for module m
     **/
    void *get_dir_config(module *m);

    /** 
     * \brief pass in a module and this method will returrn you its per-server config 
     * \param m a pointer to the static module definition
     * \return a void pointer to the config structure that was set for module m
     **/
    void *get_server_config(module *m);
    
    /**
     * \brief gives you access to the encoded password sent by the client
     * \param sent_pw a reference to a string pointer into the request where the password is
     * \return 0 (OK) if it set the 'pw' argument (and assured
     *         a correct value in r->user); otherwise it returns 
     *         an error code, either HTTP_INTERNAL_SERVER_ERROR if things are 
     *         really confused, HTTP_UNAUTHORIZED if no authentication at all 
     *         seemed to be in use, or DECLINED if there was authentication but 
     *         it wasn't Basic (in which case, the caller should presumably 
     *         decline as well).
     **/
    int get_basic_auth_pw(const char **sent_pw);

    /**
     * \brief this method writes a character for this request
     * \param c the character to write
     * \return the number of bytes sent
     **/
    int rputc(int c);

    /**
     * \brief this method writes a string for this request
     * \param c the character to write
     * \return the number of bytes sent
     **/
    int rputs(const char *str) const;

    /**
     * \brief writes a buffer for this request
     * \param buf the content to write
     * \param nbyte the number of bytes to send from buf
     * \return the number of bytes sent
     **/
    int rwrite(const void *buf, int nbyte);

    /**
     * \brief a printf style way to write data to the request
     * \param fmt The format string
     * \param ... Thee arguments used to fill the format string
     * \return the number of bytes sent
     **/
    int rprintf(const char *fmt, ...);

    /**
     * \brief Flushn all the data for this request to the client
     * \return number of bytes sent
     **/
    int rflush();
    
    /**
     * \brief the allow options tell you what options are set for this request:
     * indexes, includes, sym links, execcgi
     * \return a bitmap of the options set
     **/
    inline    int allow_options() { return ap_allow_options(mRequest); }

    /** \return bitmask of the allowoverrides for this request **/
    inline    int allow_overrides(){ return ap_allow_overrides(mRequest); }

    /** \return default type from the configuration, or text/plain if not set **/
    inline    const char *default_type() { return ap_default_type(mRequest); } 

    /**
     * \brief WARNING: This is in to be backward compatible, but is not always 
     * acurate (e.g. if mod_userdir is enabled).  This should not be used
     * \return the document root from the configuration (not necessarily the active one for this request)
     **/
    inline    const char *document_root() { return ap_document_root(mRequest); }

    /** \return Lookup the login name of the remote user.  Undefined if it cannot be determined **/
    inline    const char *get_remote_logname() { return ap_get_remote_logname(mRequest); }

    /** \return the server name from the reuqest **/
    inline    const char *get_server_name() { return ap_get_server_name(mRequest); }

    /**
     * \brief Install a custom response handler for a given status)
     * \param status the status to hook into
     * \param str the custom response - it can be a static string, the full path to a file, or a URL
     **/
    inline    void custom_response(int status, const char *str)
        { ap_custom_response(mRequest, status, (char *)str); }
    
    /**
     * \brief run an internal redirect to another URI in this server
     * \param new_uri the new uri to fire
     **/
    inline    void internal_redirect(const char *new_uri)
        { ap_internal_redirect(new_uri, mRequest); }
    
    /**
     * \brief This function is designed for things like actions or CGI scripts, when
     * using AddHandler, and you want to preserve the content type across
     * an internal redirect.
     * \param new_uri the URI to replace the current request with
     **/
    inline    void internal_redirect_handler(const char *new_uri)
    { ap_internal_redirect(new_uri, mRequest); }
    
    inline    int some_auth_required()
    { return ap_some_auth_required(mRequest); }
    
    inline    int is_initial_req()
    { return ap_is_initial_req(mRequest); }
    
    inline    void update_mtime(apr_time_t dependency_mtime)
    { ap_update_mtime(mRequest, dependency_mtime); }
    
    void allow_methods(int reset, ...);
    
	inline    void	get_mime_headers() { ap_get_mime_headers(mRequest); }
	inline    void	get_mime_headers_core( apr_bucket_brigade *bb) { ap_get_mime_headers_core(mRequest, bb); }
	inline    void	finalize_request_protocol(){ ap_finalize_request_protocol(mRequest); }
	inline    void	send_error_response(int recursive_error){ ap_send_error_response(mRequest, recursive_error); }
	inline    void	set_content_length(apr_off_t length){ap_set_content_length(mRequest, length); }
	inline    int	set_keepalive(){ return ap_set_keepalive(mRequest); }
	inline    apr_time_t rationalize_mtime(apr_time_t mtime){ return ap_rationalize_mtime(mRequest, mtime); }
	inline    const char * make_content_type(const char *type){ return ap_make_content_type(mRequest, type); }
	inline    char *make_etag(int force_weak){ return ap_make_etag(mRequest,  force_weak); }
	inline    void	set_etag(){ap_set_etag(mRequest); }
	inline    void	set_last_modified(){ap_set_last_modified(mRequest); }
	inline    int	meets_conditions(){ return ap_meets_conditions(mRequest); }
	inline    void	set_content_type(const char *ct){ap_set_content_type(mRequest, ct); }
	inline    int	setup_client_block(int read_policy){ return ap_setup_client_block(mRequest, read_policy); }
	inline    int	should_client_block(){ return ap_should_client_block(mRequest); }
	inline    void	note_auth_failure(){ap_note_auth_failure(mRequest); }
	inline    void	note_basic_auth_failure(){ap_note_basic_auth_failure(mRequest); }
	inline    void	note_digest_auth_failure(){ap_note_digest_auth_failure(mRequest); }
	inline    void	parse_uri(const char *uri){ap_parse_uri(mRequest, uri); }
	
	void dump() const;
    string dump_string() const;
    string table_string(const apr_table_t *pTable) const;
    void dump_table(const apr_table_t *pTable) const;
    request_rec *get_request_rec() { return mRequest; }
};

#endif /* CPP_REQUEST_H */
