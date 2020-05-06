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

#include "tt/parser/line/array_parser.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

#include "tt/parser/define.hpp"
#include "tt/parser/line/exception.hpp"
#include "tt/parser/utils.hpp"

using std::string;

using tt::ArrayParser;
using tt::EmptyArrayException;
using tt::EmptyKeyException;
using tt::ValuesAfterEndingTokenException;

using tt::kArrayCloseToken;
using tt::kArrayOpenToken;
using tt::kAssignmentToken;

bool ArrayParser::StartParsing(const string &line) {
    assert(!IsParsing());

    auto equal_token_pos = line.find(kAssignmentToken);
    if (equal_token_pos == string::npos) {
        return false;
    }
    auto parenthesis_pos = line.find(kArrayOpenToken, equal_token_pos + 1);
    if (parenthesis_pos == string::npos) {
        return false;
    }

    auto tmp =
        line.substr(equal_token_pos + 1, parenthesis_pos - equal_token_pos - 1);
    trim(tmp);
    if (!tmp.empty()) {
        return false;
    }

    key_ = line.substr(0, equal_token_pos);
    trim(key_);
    if (key_.empty()) {
        throw EmptyKeyException();
    }

    is_parsing_ = true;

    UpdateStatus(line.substr(parenthesis_pos + 1, string::npos));

    return true;
}

void ArrayParser::ParseLine(const string &line) {
    assert(IsParsing());
    UpdateStatus(line);
}

void ArrayParser::UpdateStatus(const string &line) {
    auto trimmed_line = trim_copy(line);
    if (line.empty()) {
        return;
    }
    auto ending_token_pos = trimmed_line.find(kArrayCloseToken);
    if (ending_token_pos != string::npos) {
        is_parsing_ = false;
        // There shall be no character after the kArrayCloseToken
        if (ending_token_pos + 1 != trimmed_line.size()) {
            const auto msg = key_ + ": No value allowed after ending token '" +
                             string{kArrayCloseToken} + "'";
            throw ValuesAfterEndingTokenException(msg);
        }
    }
    AddValuesFromLine(trimmed_line.substr(0, ending_token_pos));

    if (!IsParsing() && values_.empty()) {
        const auto msg = key_ + ": Empty array is not allowed";
        throw EmptyArrayException(msg);
    }
}

void ArrayParser::AddValuesFromLine(const string &line) {
    std::istringstream iss(line);
    auto old_end = std::end(values_);
    values_.insert(old_end, std::istream_iterator<string>{iss},
                   std::istream_iterator<string>());
}
