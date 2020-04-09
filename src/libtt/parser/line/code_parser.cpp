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

#include "libtt/parser/line/code_parser.hpp"

#include <string>

#include "libtt/parser/line/exception.hpp"
#include "libtt/parser/utils.hpp"

using std::string;
using tt::CodeParser;

bool CodeParser::StartParsing(const string line) {
    if (IsParsing()) {
        throw tt::CodeParserIsStillParsingException();
    }
    auto trimmed_line = trim_copy(line);
    auto equal_token_pos = trimmed_line.find('=');
    if (equal_token_pos == string::npos) {
        return false;
    }

    if (equal_token_pos + 1 == trimmed_line.size() ||
        trimmed_line[equal_token_pos + 1] != '(') {
        return false;
    }

    // Are there other chars after '('?
    if (equal_token_pos + 2 != trimmed_line.size()) {
        return false;
    }

    key_ = trimmed_line.substr(0, equal_token_pos - 1);
    rtrim(key_);
    return is_parsing_ = true;
}

void CodeParser::ParseLine(const string line) {
    auto trimmed_line = trim_copy(line);
    if (trimmed_line == ")") {
        is_parsing_ = false;
        return;
    }

    code_ << line;
    code_ << "\n";
}

const string CodeParser::code() const {
    if (IsParsing()) {
        throw CodeParserIsStillParsingException();
    }

    auto code = code_.str();
    // Remove last char which is a newline added by ParseLine
    code.pop_back();
    return code;
}
