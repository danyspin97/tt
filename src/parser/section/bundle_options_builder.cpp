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

#include <cassert> // for assert

#include "tt/data/bundle_options.hpp"      // for BundleOptions
#include "tt/parser/line/array_parser.hpp" // for ArrayParser
#include "tt/parser/parser_error.hpp"      // for ParserError
#include "tt/parser/section/exception.hpp" // for SectionBuilderException
#include "tt/parser/section/utils.hpp"     // for AttributeNotFound, IsEmpt...

tt::BundleOptionsBuilder::BundleOptionsBuilder() : SectionBuilder("options") {}

auto tt::BundleOptionsBuilder::GetValidArrayAttributes() const
    -> std::vector<std::string> {
    return {"contents", "dependencies"};
}

auto tt::BundleOptionsBuilder::EndParsing() -> tl::expected<void, ParserError> {
    if (auto ret = SectionBuilder::EndParsing(); !ret.has_value()) {
        return ret;
    }

    auto contents = GetArrayAttribute("contents");
    if (!contents.has_value() || contents.value().empty()) {
        return make_parser_error<void>(
            ParserError::Type::BundleEmptyContents,
            "Bundle cannot have empty contents in [options] section");
    }
    options_.contents(contents.value());

    if (auto dependencies = GetArrayAttribute("dependencies");
        dependencies.has_value()) {
        options_.dependencies(dependencies.value());
    }

    return {};
}

auto tt::BundleOptionsBuilder::options() -> BundleOptions && {
    return std::move(options_);
}
