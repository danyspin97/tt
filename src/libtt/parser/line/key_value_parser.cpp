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

#include "libtt/parser/line/key_value_parser.hpp"

#include "libtt/parser/line/exception.hpp"
#include "libtt/parser/utils.hpp"

using std::string;

tt::KeyValueParser::KeyValueParser(string line, bool throw_on_error) {
    line_ = line;
    throw_on_error_ = throw_on_error;
    ParseLine();
}

void tt::KeyValueParser::ParseLine() {
    if (TryParseLine()) {
        valid_ = true;
    } else {
        if (throw_on_error_) {
            const auto msg = "Could not find token '=' in line `" + line_ + "`";
            throw new tt::KeyValueParserLineInvalidException(msg);
        } else {
            valid_ = false;
        }
    }
}

bool tt::KeyValueParser::TryParseLine() {
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
