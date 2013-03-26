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

class MODCPP_API env_bucket_entry {
private: 
    std::string _key;
    std::string _value;

public:
    env_bucket_entry(std::string key, std::string value) { _key = key; _value = value; }

    unsigned long hash_code();
    std::string get_value() { return _value; }
    std::string get_key() { return _key; }
};

class MODCPP_API env_hash {
private:
    std::vector< std::vector<env_bucket_entry *> *> _buckets;

public:

    env_hash();
    std::string get(std::string key);
    void put(std::string key, std::string value);
 
};


#endif /* __ENV_VALUE_H__ */
