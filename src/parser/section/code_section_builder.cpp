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

#include "tt/parser/section/code_section_builder.hpp"

#include <string>  // for allocator, operator+
#include <utility> // for move

#include "tt/parser/line/code_parser.hpp"      // for CodeParser
#include "tt/parser/line/key_value_parser.hpp" // for KeyValueParser
#include "tt/parser/section/exception.hpp"     // for SectionBuilderException
#include "tt/parser/section/utils.hpp"         // for SetThrowsIfNotEmpty

tt::CodeSectionBuilder::CodeSectionBuilder(std::string section)
    : section_(std::move(section)) {}

void tt::CodeSectionBuilder::ParseLine(const std::string &line) {
    if (code_parser_.IsParsing()) {
        // Continue parsing execute parameter
        code_parser_.ParseLine(line);
        if (!code_parser_.IsParsing()) {
            SetThrowsIfNotEmpty(GetCodeAttributeForKey(code_parser_.key()),
                                code_parser_.code());
            code_parser_.Reset();
        }
        return;
    }

    if (IsEmptyLine(line)) {
        return;
    }

    if (code_parser_.StartParsing(line)) {
        return;
    }

    auto key_value_parser = KeyValueParser(line);
    if (key_value_parser.IsLineValid()) {
        auto key = key_value_parser.key();
        auto value = key_value_parser.value();
        SetThrowsIfNotEmpty(GetAttributeForKey(key), value);
        return;
    }

    const auto msg =
        line + "\n\tis not valid while parsing [" + section_ + "] section";
    throw SectionBuilderException(msg);
}
