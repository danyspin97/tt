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

#include "libtt/parser/section/main_section_builder.hpp"

#include <string>

#include "libtt/parser/line/key_value_parser.hpp"
#include "libtt/parser/section/utils.hpp"
#include "libtt/parser/utils.hpp"

using std::string;
using tt::AttributeNotFound;
using tt::KeyValueParser;
using tt::MainSection;
using tt::MainSectionBuilder;
using tt::SetThrowsIfNotEmpty;

void MainSectionBuilder::ParseLine(const string line) {
    try {
        TryParseLine(line);
    } catch (std::exception e) {
        throw std::exception(e);
    }
}

void MainSectionBuilder::TryParseLine(const string line) {
    if (IsEmptyLine(line)) {
        return;
    }

    const auto keyValueParser = KeyValueParser(line, true);
    const auto key = keyValueParser.key();
    const auto value = keyValueParser.value();

    SetThrowsIfNotEmpty(GetAttributeForKey(key), value);
}

string &MainSectionBuilder::GetAttributeForKey(const std::string key) {
    if (key == "name") {
        return main_section_.name;
    } else if (key == "description") {
        return main_section_.description;
    } else if (key == "polish_name") {
        return main_section_.polish_name;
    } else if (key == "type") {
        return main_section_.type;
    };

    AttributeNotFound(key, "main");
    return ret_;
}