/*
 * Copyright (c) 2020 Danilo Spinella <oss@danyspin97.org>.
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

#include "tt/parser/line/code_parser.hpp"

#include <string>

#include "tt/parser/define.hpp"
#include "tt/parser/line/exception.hpp"
#include "tt/utils/trim.hpp"

using std::string;

using tt::CodeParser;
using tt::kAssignmentToken;
using tt::kCodeCloseToken;
using tt::kCodeOpenToken;

auto CodeParser::StartParsing(const string &line) -> bool {
    if (IsParsing()) {
        throw tt::CodeParserIsStillParsingException();
    }
    auto trimmed_line = utils::trim_copy(line);
    auto equal_token_pos = trimmed_line.find(kAssignmentToken);
    if (equal_token_pos == string::npos) {
        return false;
    }

    if (equal_token_pos + 1 == trimmed_line.size() ||
        trimmed_line[equal_token_pos + 1] != kCodeOpenToken) {
        return false;
    }

    // Are there other chars after kCodeOpenToken?
    if (equal_token_pos + 2 != trimmed_line.size()) {
        return false;
    }

    key_ = trimmed_line.substr(0, equal_token_pos - 1);
    utils::rtrim(key_);
    return is_parsing_ = true;
}

void CodeParser::ParseLine(const string &line) {
    auto trimmed_line = utils::trim_copy(line);
    if (trimmed_line == string{kCodeCloseToken}) {
        is_parsing_ = false;
        return;
    }

    code_ << line;
    code_ << "\n";
}

auto CodeParser::code() const -> string {
    if (IsParsing()) {
        throw CodeParserIsStillParsingException();
    }

    auto code = code_.str();
    // Remove last char which is a newline added by ParseLine
    code.pop_back();
    return code;
}
