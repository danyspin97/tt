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

#ifndef TT_PARSER_SECTION_UTILS_HPP_
#define TT_PARSER_SECTION_UTILS_HPP_

#include <string> // for string, basic_string

#include "tt/parser/section/exception.hpp" // for ParamIsNotEmptyException
#include "tt/utils/trim.hpp"               // for trim_copy

namespace tl {
template <typename T, typename Z> class expected;
}

namespace tt {
class ParserError;
class SectionBuilder;

auto KeyNotFound(const std::string &attribute, const std::string &section)
    -> tl::expected<void, ParserError>;

// Is called many times during parsing, allow inlining
inline auto IsEmptyLine(const std::string &line) -> bool {
    const auto strippedLine = utils::trim_copy(line);
    return strippedLine.empty() || strippedLine[0] == '#';
}

auto TestBuilderWithFile(SectionBuilder &builder, const std::string &path)
    -> tl::expected<void, ParserError>;

} // namespace tt

#endif // TT_PARSER_SECTION_UTILS_HPP_
