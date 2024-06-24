#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <set>
#include <type_traits>

#include "boost/lexical_cast.hpp"


namespace utopian {
namespace ranker {

template<typename T>
struct is_number_type {
    static constexpr bool value = std::is_floating_point<T>::value || std::is_integral<T>::value;
};

template<typename T>
struct is_string_type {
    static constexpr bool value = std::is_same<T, std::string>::value;
};
// std::enable_if<>::type

template<typename T>
int split_new_v1(const std::string& line, const std::string& delims, std::vector<T>& tokens) {
        std::string::size_type begin;
        std::string::size_type end;
        begin = line.find_first_not_of(delims);
        int n = 0;
        while (begin != std::string::npos) {   
            end  = line.find_first_of(delims, begin);
            if (end == std::string::npos) {
                end = line.length();
            }
            if (is_number_type<T>::value) {
                T uid = 0;
                try {
                    uid = boost::lexical_cast<T>(line.substr(begin, (end - begin)));
                } catch (std::invalid_argument& e) {
                    continue;
                }
                tokens.emplace_back(uid);
            } else if (is_string_type<T>::value) {
                //opt
                T result = boost::lexical_cast<T>(line.substr(begin, (end-begin)));
                tokens.emplace_back(std::move(result));
            }
            ++n;
            begin = line.find_first_not_of(delims, end);
        }
        return n;
    }

}
    
} // namespace utopia 
