#define EXPORT_MODCPP
#include "cpp_request.h"
#include <string>   // for std::string
#include <stdio.h>  // for sprintf
#include <apr_strings.h>
#include <apr_fnmatch.h>
#include <apr_strings.h>
#include <apr_lib.h>
#include <apr_pools.h>
using std::string;

ApacheRequestRec::ApacheRequestRec(request_rec *r, ApacheRequestRec *pPrev,
                                   ApacheRequestRec *pNext)
{
    mRequest = r;
    mPrev = pPrev;
    mNext = pNext;
    if( !pPrev && r->prev ) {
        pPrev = new ApacheRequestRec(r->prev, NULL, this);
    }
    if( !pNext && r->next ) {
        pNext = new ApacheRequestRec(r->next, this, NULL);
    }
    mMain = r->main ? new ApacheRequestRec(r->main) : NULL;
    mServer = new ApacheServerRec(r->server);
}

ApacheRequestRec::~ApacheRequestRec()
{
    delete mPrev;
    delete mNext;
    delete mMain;
    delete mServer;
}

// translate integer to ascii string
string ApacheRequestRec::istring(int value, const char* format) const
{
    enum { TEMP_STORE = 50 };
    char temp[TEMP_STORE];
    const int len = apr_snprintf(temp, TEMP_STORE, format, value);
    return string(temp, len);
}


// translate non-NULL char* to string, NULL to empty string
string ApacheRequestRec::mstring(const char *cp) const
{
    return cp ? cp : string();
}

string ApacheRequestRec::dump_string() const
{
    return string("============ ApacheRequestRec ==============\n")

	+ "the_request: "       +      mstring(this->the_request()) + "\n"
	+ "content_type: "      +      mstring(this->content_type()) + "\n"
	+ "assbackwards: "      +      istring(this->assbackwards()) + "\n"
	+ "proxyreq: "          +      istring(this->proxyreq()) + "\n"
	+ "header_only: "       +      istring(this->header_only()) + "\n"
	+ "protocol: "          +      mstring(this->protocol()) + "\n"
	+ "pro  inum: "         +      istring(this->proto_num()) + "\n"
	+ "hostname: "          +      mstring(this->hostname()) + "\n"
	+ "status_line: "       +      mstring(this->status_line()) + "\n"
	+ "status: "            +      istring(this->status()) + "\n"
	+ "method: "            +      mstring(this->method()) + "\n"
	+ "method_number: "     +      istring(this->method_number()) + "\n"
	+ "allowed: "           +      istring(this->allowed()) + "\n"
	+ "bytes_sent: "        +      istring(this->bytes_sent()) + "\n"
	+ "args: "              +      mstring(this->args()) + "\n"
	+ "headers_in: \n"      + table_string(this->headers_in())
	+ "headers_out: \n"     + table_string(this->headers_out())
	+ "err_headers_out: \n" + table_string(this->err_headers_out())
	+ "subprocess_env: \n"  + table_string(this->subprocess_env())
	+ "notes: \n"           + table_string(this->notes())

	+ "============ /ApacheRequestRec ==============\n";
}

void ApacheRequestRec::dump() const
{
    this->rputs(dump_string().c_str());
}

string ApacheRequestRec::table_string(const apr_table_t *pTable) const
{
    string ret_val;
    apr_table_entry_t *pEntry = (apr_table_entry_t *)
        ((apr_array_header_t *)pTable)->elts;
    for (int i = 0, n = ((apr_array_header_t *)pTable)->nelts; i < n; i++) {
	ret_val += "    [" +   istring(i) + "] '" + pEntry[i].key + "'='"
	    + pEntry[i].val + "'\n";
    }
    return ret_val;
}

void ApacheRequestRec::dump_table(const apr_table_t *pTable) const
{
    rputs(table_string(pTable).c_str());
}

void *ApacheRequestRec::get_dir_config(module *m)
{
    return ap_get_module_config(mRequest->per_dir_config, m);
}

int ApacheRequestRec::get_basic_auth_pw(const char **sent_pw)
{
    return ap_get_basic_auth_pw(mRequest, sent_pw);
}

int ApacheRequestRec::rputs(const char *str) const
{
    return ap_rputs(str, mRequest);
}

int ApacheRequestRec::rputc(int c)
{
    return ap_rputc(c, mRequest);
}

int ApacheRequestRec::rwrite(const void *buf, int nbyte)
{
    return ap_rwrite(buf, nbyte, mRequest);
}

int ApacheRequestRec::rprintf(const char *fmt, ...)
{
    int result;
    va_list vp;
    va_start(vp, fmt);
    result = ap_vrprintf(mRequest, fmt, vp);
    va_end(vp);
    return result;
}

int ApacheRequestRec::rflush()
{
    return ap_rflush(mRequest);
}

void ApacheRequestRec::allow_methods(int reset, ...)
{
    va_list vp;
    va_start(vp, reset);
    ap_allow_methods(mRequest, reset, vp);
    va_end(vp);
}
