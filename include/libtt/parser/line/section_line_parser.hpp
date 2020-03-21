// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#ifndef LIBTT_SECTION_LINE_PARSER_HPP_
#define LIBTT_SECTION_LINE_PARSER_HPP_

#include <string>

namespace tt {

class SectionLineParser {
public:
    const std::string section() const {
        if (valid_) {
            return section_;
        }
        throw new std::exception();
    }

    bool line_valid() const { return valid_; }

    SectionLineParser(const std::string line) {
        line_ = line;
        ParseLine();
    }

    const std::string GetSectionOrDefault(std::string _default) const {
        if (line_valid()) {
            return section_;
        }

        return _default;
    }

private:
    void ParseLine();

    std::string line_;
    std::string section_;
    bool valid_;
};

} // namespace tt

#endif // LIBTT_KEY_VALUE_PARSER_HPP_
