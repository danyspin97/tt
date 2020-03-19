// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#ifndef LIBTT_PARSER_UTILS_HPP_
#define LIBTT_PARSER_UTILS_HPP_

// Thanks StackOverflow!
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

#include <algorithm> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string &s) {
s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
return !std::isspace(ch);
}));
    }
    // trim from end (in place)
     static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
    return !std::isspace(ch);
    }).base(), s.end());
    }
        // trim from both ends (in place)
    static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
    }

#endif // LIBTT_PARSER_UTILS_HPP_

