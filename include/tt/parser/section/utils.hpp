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

#include <string>

#include "tt/parser/section/exception.hpp"
#include "tt/parser/section/section_builder.hpp"
#include "tt/parser/utils.hpp"

namespace tt {

void AttributeNotFound(const std::string &attribute,
                       const std::string &section);

// Is called many times during parsing, allow inlining
inline auto IsEmptyLine(const std::string &line) -> bool {
    const auto strippedLine = trim_copy(line);
    return strippedLine.empty() || strippedLine[0] == '#';
}

inline void SetThrowsIfNotEmpty(std::string &param,
                                const std::string &newValue) {
    if (!param.empty()) {
        throw ParamIsNotEmptyException();
    }

    param.append(newValue);
}

void TestBuilderWithFile(SectionBuilder &builder, const std::string &path);

} // namespace tt

#endif // TT_PARSER_SECTION_UTILS_HPP_
