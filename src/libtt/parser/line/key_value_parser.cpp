// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#include "parser/line/key_value_parser.hpp"

#include "parser/utils.hpp"

using std::string;

namespace tt {

KeyValueParser::KeyValueParser(string line, bool throw_on_error) {
    line_ = line;
    throw_on_error_ = throw_on_error;
    parseLine();
}

void KeyValueParser::parseLine() {
    if (tryParseLine()) {
        valid_ = true;
    } else {
        if (throw_on_error_) {
            const auto msg = "Could not find token '=' in line `" + line_ + "`";
            throw new std::exception();
        } else {
            valid_ = false;
        }
    }
}

bool KeyValueParser::tryParseLine() {
    auto token_pos = line_.find('=');
    if (token_pos == string::npos) {
        return false;
    }
    key_ = line_.substr(0, token_pos);
    trim(key_);
    value_ = line_.substr(token_pos + 1, string::npos);
    trim(value_);
    return true;
}

}  // namespace tt
