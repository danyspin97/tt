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
