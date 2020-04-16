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

#include "tt/parser/section/longrun_options_builder.hpp"

#include "tt/parser/section/utils.hpp"
#include "tt/parser/utils.hpp"
#include "tt/signal.hpp"

using std::stoi;
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
    if (key == "down_signal") {
        options_.down_signal(ParseSignalFromString(value));
    } else if (key == "maxdeath") {
        options_.max_death(stoi(value));
    } else if (key == "notify") {
        options_.notify(stoi(value));
    } else if (key == "optional") {
        options_.optional(ParseBoolean(value));
    } else if (key == "timeout_finish") {
        options_.timeout_finish(stoi(value));
    } else if (key == "timeout_kill") {
        options_.timeout_kill(stoi(value));
    } else if (key == "write_message") {
        options_.write_run_finish_message(ParseBoolean(value));
    } else {
        AttributeNotFound(key, "options");
    }
}
