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
#include "tt/parser/section/exception.hpp"     // for SectionBuilderException
#include "tt/parser/section/main_section.hpp"  // for MainSection
#include "tt/parser/section/utils.hpp"         // for AttributeNotFound

void tt::MainSectionBuilder::ParseLine(const std::string &line) {
    try {
        TryParseLine(line);
    } catch (const AttributeIsAlreadySetException & /*e*/) {
        const auto msg =
            "\"" + key_ + "\" was already set while parsing [main] section";
        throw SectionBuilderException(msg);
    } catch (const AttributeNotFoundException &e) {
        throw SectionBuilderException(e.msg());
    }
}

void tt::MainSectionBuilder::TryParseLine(const std::string &line) {
    if (IsEmptyLine(line)) {
        return;
    }

    const auto keyValueParser = KeyValueParser(line, true);
    key_ = keyValueParser.key();
    value_ = keyValueParser.value();

    SetThrowsIfNotEmpty(GetAttributeForKey(key_), value_);
}

auto tt::MainSectionBuilder::GetAttributeForKey(const std::string &key)
    -> std::string & {
    if (key == "name") {
        return main_section_.name;
    }
    if (key == "description") {
        return main_section_.description;
    }
    if (key == "type") {
        return main_section_.type;
    }

    AttributeNotFound(key, "main");
}

auto tt::MainSectionBuilder::main_section() -> MainSection & {
    return main_section_;
}
