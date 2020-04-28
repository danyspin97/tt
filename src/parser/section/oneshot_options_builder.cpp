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

#include "tt/parser/section/oneshot_options_builder.hpp"

#include "tt/data/oneshot_options.hpp"
#include "tt/parser/section/options_builder.hpp"
#include "tt/parser/section/utils.hpp"
#include "tt/parser/utils.hpp"

using std::string;

using tt::ArrayParser;
using tt::AttributeNotFound;
using tt::OneshotOptions;
using tt::OneshotOptionsBuilder;
using tt::ParseBoolean;

OneshotOptionsBuilder::OneshotOptionsBuilder(OneshotOptions &options)
    : options_(options) {}

void OneshotOptionsBuilder::TrySetAttributeForKey(const string &key,
                                                  const string &value) {
    if (key == "optional") {
        options_.optional(ParseBoolean(value));
        return;
    } else if (key == "write_message") {
        options_.write_start_stop_messages(ParseBoolean(value));
        return;
    }

    AttributeNotFound(key, "options");
}

void OneshotOptionsBuilder::SaveValuesOfParser(const ArrayParser &parser) {
    const auto key = parser.key();
    if (key == "depends") {
        options_.dependencies(parser.values());
        return;
    }

    AttributeNotFound(key, "options");
}