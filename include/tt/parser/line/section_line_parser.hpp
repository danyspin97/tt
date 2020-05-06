/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
 *
 * This file is part of tt
 * (see https://github.com/danyspin97/tt).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TT_SECTION_LINE_PARSER_HPP_
#define TT_SECTION_LINE_PARSER_HPP_

#include <string>

namespace tt {

class SectionLineParser {
public:
    std::string section() const { return section_; }
    bool IsLineValid() const { return valid_; }

    SectionLineParser(const std::string &line) {
        line_ = line;
        ParseLine();
    }

    std::string GetSectionOrDefault(std::string _default) const {
        if (IsLineValid()) {
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

#endif // TT_SECTION_LINE_PARSER_HPP_
