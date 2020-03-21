// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#ifndef LIBTT_CODE_PARSER_HPP_
#define LIBTT_CODE_PARSER_HPP_

#include <sstream>
#include <string>

namespace tt {

class CodeParser {
public:
    bool IsParsing() const { return is_parsing_; }

    const std::string code() const {
        if (IsParsing()) {
            throw new std::exception();
        }

        return code_.str();
    }

    const std::string key() const { return key_; }

    bool StartParsing(const std::string line);

    void ParseLine(const std::string line);

    void Reset() {
        code_ = std::stringstream();
        is_parsing_ = false;
        key_ = "";
    }

private:
    std::stringstream code_;
    std::string key_;
    bool is_parsing_ = false;
};

} // namespace tt

#endif // LIBTT_CODE_PARSER_HPP_
