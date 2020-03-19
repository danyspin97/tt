// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#ifndef LIBTT_KEY_VALUE_PARSER_HPP_
#define LIBTT_KEY_VALUE_PARSER_HPP_

#include <string>

namespace tt {

class KeyValueParser {
public:
    const std::string getKey() { return key_; }

    const std::string getValue() { return value_; }

    bool isLineValid() { return valid_; }

    KeyValueParser(std::string line, bool throw_on_error = false);

private:
    void parseLine();
    bool tryParseLine();

    std::string line_;
    std::string key_;
    std::string value_;
    bool valid_;
    bool throw_on_error_;
};

}  // namespace tt

#endif  // LIBTT_KEY_VALUE_PARSER_HPP_
