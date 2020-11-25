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

#include "fmt/format.h" // for format

#include "tl/expected.hpp" // for expected

#include "tt/parser/line/code_parser.hpp"      // for CodeParser
#include "tt/parser/line/key_value_parser.hpp" // for KeyValueParser
#include "tt/parser/parser_error.hpp"          // for ParserError
#include "tt/parser/section/exception.hpp"     // for SectionBuilderException
#include "tt/parser/section/utils.hpp"         // for SetThrowsIfNotEmpty

auto tt::CodeSectionBuilder::ParseLine(const std::string &line) noexcept
    -> tl::expected<void, ParserError> {
    if (code_parser_.IsParsing()) {
        // Continue parsing execute parameter
        code_parser_.ParseLine(line);
        if (!code_parser_.IsParsing()) {
            auto ret = SetCodeAttribute();
            if (!ret.has_value()) {
                return chain_parser_error<void>(
                    ret.error(), fmt::format("in [{}] section", section()));
            }
            code_parser_.Reset();
        }
        return {};
    }

    if (IsEmptyLine(line)) {
        return {};
    }

    auto ret = code_parser_.StartParsing(line);
    if (!ret.has_value()) {
        return tl::unexpected(ret.error());
    }

    if (ret.value()) {
        return {};
    }

    return SectionBuilder::ParseLine(line);
}

auto tt::CodeSectionBuilder::SetCodeAttribute() noexcept
    -> tl::expected<void, ParserError> {
    auto key = code_parser_.key();
    auto valid_keys = GetValidCodeAttributes();
    if (std::find(valid_keys.begin(), valid_keys.end(), key) ==
        valid_keys.end()) {
        return make_parser_error<void>(
            ParserError::Type::CodeSectionInvalidKey,
            fmt::format("Key '{}' does not accept code", key));
    }

    return SetAttribute(key, code_parser_.code());
}

auto tt::CodeSectionBuilder::EndParsing() noexcept
    -> tl::expected<void, ParserError> {
    if (auto ret = SectionBuilder::EndParsing(); !ret.has_value()) {
        return ret;
    }

    if (code_parser_.IsParsing()) {
        return make_parser_error<void>(
            ParserError::Type::ArrayValueNotClosed,
            fmt::format("Array value '{}' has not been closed",
                        code_parser_.key()));
    }

    return {};
}
