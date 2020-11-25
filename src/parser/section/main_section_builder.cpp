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

#include "tt/parser/section/main_section_builder.hpp"

#include "tt/parser/line/key_value_parser.hpp" // for KeyValueParser
#include "tt/parser/parser_error.hpp"          // for ParserError
#include "tt/parser/section/exception.hpp"     // for SectionBuilderException
#include "tt/parser/section/main_section.hpp"  // for MainSection
#include "tt/parser/section/utils.hpp"         // for AttributeNotFound

tt::MainSectionBuilder::MainSectionBuilder() noexcept
    : SectionBuilder("main") {}

auto tt::MainSectionBuilder::GetValidAttributes() const noexcept
    -> std::vector<std::string> {
    return {"name", "description", "type"};
}

auto tt::MainSectionBuilder::EndParsing() noexcept
    -> tl::expected<void, ParserError> {
    if (auto ret = SectionBuilder::EndParsing(); !ret.has_value()) {
        return ret;
    }

    auto name = GetAttribute("name");
    if (!name.has_value()) {
        return make_parser_error<void>(
            ParserError::Type::MissingValueInMainSection,
            "Missing name in [main] section");
    }
    main_section_.name = std::move(name.value());

    auto description = GetAttribute("description");
    if (!description.has_value()) {
        return make_parser_error<void>(
            ParserError::Type::MissingValueInMainSection,
            "Missing description in [main] section");
    }
    main_section_.description = std::move(description.value());

    auto type = GetAttribute("type");
    if (!type.has_value()) {
        return make_parser_error<void>(
            ParserError::Type::MissingValueInMainSection,
            "Missing type in [main] section");
    }

    main_section_.type = std::move(type.value());
    return {};
}

auto tt::MainSectionBuilder::main_section() noexcept -> MainSection & {
    return std::ref(main_section_);
}
