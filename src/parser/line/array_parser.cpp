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

#include "tt/parser/line/array_parser.hpp"

#include <algorithm> // for max
#include <cassert>   // for assert
#include <iterator>  // for istream_iterator, operator!=
#include <sstream>   // for basic_istream, istringstream
#include <string>    // for string, allocator, operator+

#include "fmt/format.h" // for format

#include "tt/parser/define.hpp"         // for kArrayCloseToken, kArrayOpen...
#include "tt/parser/line/exception.hpp" // for EmptyArrayException, ValuesA...
#include "tt/parser/parser_error.hpp"   // for ParserError
#include "tt/utils/trim.hpp"            // for trim, trim_copy

auto tt::ArrayParser::StartParsing(const std::string &line)
    -> tl::expected<bool, ParserError> {
    assert(!IsParsing());

    auto equal_token_pos = line.find(kAssignmentToken);
    if (equal_token_pos == std::string::npos) {
        return false;
    }
    auto parenthesis_pos = line.find(kArrayOpenToken, equal_token_pos + 1);
    if (parenthesis_pos == std::string::npos) {
        return false;
    }

    auto tmp =
        line.substr(equal_token_pos + 1, parenthesis_pos - equal_token_pos - 1);
    utils::trim(tmp);
    if (!tmp.empty()) {
        return false;
    }

    key_ = line.substr(0, equal_token_pos);
    utils::trim(key_);
    if (key_.empty()) {
        return make_parser_error<bool>(ParserError::Type::EmptyKey,
                                       "Found empty key");
    }

    is_parsing_ = true;
    auto ret = ParseLine(line.substr(parenthesis_pos + 1, std::string::npos));
    if (!ret.has_value()) {
        return tl::unexpected(ret.error());
    }
    return true;
}

auto tt::ArrayParser::ParseLine(const std::string &line)
    -> tl::expected<void, ParserError> {
    assert(IsParsing());

    auto trimmed_line = utils::trim_copy(line);
    if (line.empty()) {
        return {};
    }
    auto ending_token_pos = trimmed_line.find(kArrayCloseToken);
    if (ending_token_pos != std::string::npos) {
        is_parsing_ = false;
        // There shall be no character after the kArrayCloseToken
        if (ending_token_pos + 1 != trimmed_line.size()) {
            return make_parser_error<void>(
                ParserError::Type::ArrayValueAfterClosingToken,
                fmt::format("Found value after closing token '{}' for '{}'",
                            std::string{kArrayCloseToken}, key_));
        }
    }
    AddValuesFromLine(trimmed_line.substr(0, ending_token_pos));

    if (!IsParsing() && values_.empty()) {
        return make_parser_error<void>(
            ParserError::Type::ArrayValueIsEmpty,
            fmt::format("Array '{}' is empty", key_));
    }

    return {};
}

void tt::ArrayParser::AddValuesFromLine(const std::string &line) {
    std::istringstream iss(line);
    auto old_end = std::end(values_);
    values_.insert(old_end, std::istream_iterator<std::string>{iss},
                   std::istream_iterator<std::string>());
}
