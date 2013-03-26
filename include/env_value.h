#ifndef __ENV_VALUE_H__
#define __ENV_VALUE_H__

#ifdef WIN32
#ifdef EXPORT_MODCPP
#define MODCPP_API __declspec(dllexport)
#else
#define MODCPP_API __declspec(dllimport)
#endif /* EXPORT_MODCPP */
#else
#define MODCPP_API
#endif /* WIN32 */

#include <vector>
#include "request_env.h"
#include <iterator>

class MODCPP_API env_value {
    friend class request_env;
 public:
    typedef std::vector<std::string>::const_iterator const_iterator;
    
    env_value(const std::string &sKey = "");

    const_iterator end();
    const_iterator begin();
    int num_vals();
    std::string operator[]( int index );
    inline std::string key() { return key_; }
    
 private:
    void add_value( const std::string& value );
    std::vector<std::string> vals_;
    std::string key_;
};


#endif /* __ENV_VALUE_H__ */
