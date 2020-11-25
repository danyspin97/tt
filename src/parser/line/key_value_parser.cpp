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

#include "tt/parser/line/key_value_parser.hpp"

#include <utility> // for move

#include "fmt/format.h" // for format

#include "tt/parser/define.hpp"         // for kAssignmentToken
#include "tt/parser/line/exception.hpp" // for KeyValueParserLineInvalidExc...
#include "tt/parser/parser_error.hpp"   // for ParserError
#include "tt/utils/trim.hpp"            // for trim

auto tt::KeyValueParser::ParseLine(std::string_view line) noexcept
    -> tl::expected<std::pair<std::string, std::string>, ParserError> {
    auto token_pos = line.find(kAssignmentToken);
    if (token_pos == std::string::npos) {
        return make_parser_error<std::pair<std::string, std::string>>(
            ParserError::Type::InvalidLine, "Invalid line");
    }
    std::string key{line.substr(0, token_pos)};
    utils::trim(key);
    std::string value{line.substr(token_pos + 1, std::string::npos)};
    utils::trim(value);
    return std::make_pair(std::move(key), std::move(value));
}
