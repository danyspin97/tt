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

#include "tt/parser/section/bundle_options_builder.hpp"

#include "tt/data/bundle_options.hpp"      // for BundleOptions
#include "tt/parser/line/array_parser.hpp" // for ArrayParser
#include "tt/parser/section/exception.hpp" // for SectionBuilderException
#include "tt/parser/section/utils.hpp"     // for AttributeNotFound, IsEmpt...

void tt::BundleOptionsBuilder::ParseLine(const std::string &line) {
    if (IsEmptyLine(line)) {
        return;
    }

    if (ParseMultilineValue(line)) {
        return;
    }

    throw SectionBuilderException(line + " : not valid in section [options]");
}

void tt::BundleOptionsBuilder::SaveValuesOfParser(const ArrayParser &parser) {
    const auto &key = parser.key();
    if (key == "contents") {
        options_.contents(parser.values());
    } else if (key == "dependencies") {
        options_.dependencies(parser.values());
    } else {
        AttributeNotFound(parser.key(), "options");
    }
}

void tt::BundleOptionsBuilder::TrySetAttributeForKey(
    const std::string & /*key*/, const std::string & /*value*/) {
    throw SectionBuilderException("No key is allowed in section [options]");
}

void tt::BundleOptionsBuilder::EndParsing() {
    OptionsBuilder::EndParsing();
    if (options_.contents().empty()) {
        throw SectionBuilderException(
            "Bundle cannot have empty contents in section [options]");
    }
}

auto tt::BundleOptionsBuilder::options() -> BundleOptions && {
    return std::move(options_);
}
