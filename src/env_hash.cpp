#define EXPORT_MODCPP
#ifndef NO_STDCPP

#include "env_hash.h"

static const int NUM_BUCKETS = 1000;

unsigned long env_bucket_entry::hash_code() {
    const char *str = _key.c_str();
    unsigned long hash = 5381;
    int c;

    while( (c = *str++) ) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

env_hash::env_hash() {
    for( int i = 0; i < NUM_BUCKETS; i++ ) {
        _buckets[i] = new std::vector<env_bucket_entry *>();
    }
}

std::string env_hash::get(std::string key) {
    std::string result;
    env_bucket_entry *entry = new env_bucket_entry(key, "");
    std::vector<env_bucket_entry *> *bucket = _buckets[entry->hash_code() & NUM_BUCKETS]; 
    std::vector<env_bucket_entry *>::iterator cur;
    for( cur = bucket->begin(); cur < bucket->end(); cur++ ) {
        env_bucket_entry *cur_entry = (*cur);
        if( cur_entry->get_key() == key ) {
            result = cur_entry->get_value();
            break;
        }
    }
    return result; 
}

void env_hash::put(std::string key, std::string value) {
    env_bucket_entry *entry = new env_bucket_entry(key, value);
    std::vector<env_bucket_entry *> *bucket = _buckets[entry->hash_code() & NUM_BUCKETS];
    bucket->push_back(entry);
}
#endif
