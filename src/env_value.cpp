#define EXPORT_MODCPP
#ifndef NO_STDCPP

#include "env_value.h"

env_value::env_value(const string &sKey) {
    key_ = sKey;
}

int
env_value::num_vals(){
    return vals_.size();
}

std::string
env_value::operator[](int index){
    const unsigned uindex = index;
    if ( uindex < vals_.size() )
        return vals_[uindex];
    else
        return std::string();
}

void
env_value::add_value( const std::string& value ){
    vals_.push_back(value);
}

env_value::const_iterator
env_value::end(){
    return vals_.end();
}

env_value::const_iterator
env_value::begin(){
    return vals_.begin();
};
#endif
