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

#include "tt/parser/section/longrun_options_builder.hpp"

#include "tt/data/longrun_options.hpp"     // for LongrunOptions
#include "tt/parser/line/array_parser.hpp" // for ArrayParser
#include "tt/parser/section/utils.hpp"     // for AttributeNotFound
#include "tt/utils/parse_boolean.hpp"      // for ParseBoolean

using std::string;

using tt::AttributeNotFound;
using tt::LongrunOptions;
using tt::LongrunOptionsBuilder;

LongrunOptionsBuilder::LongrunOptionsBuilder(LongrunOptions &options)
    : options_(options) {}

void LongrunOptionsBuilder::SaveValuesOfParser(const ArrayParser &parser) {
    const auto key = parser.key();
    if (key == "depends") {
        options_.dependencies(parser.values());
        return;
    }

    AttributeNotFound(key, "options");
}

void LongrunOptionsBuilder::TrySetAttributeForKey(const string &key,
                                                  const string &value) {
    if (key == "optional") {
        options_.optional(utils::ParseBoolean(value));
    } else {
        AttributeNotFound(key, "options");
    }
}
