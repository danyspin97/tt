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

#include "tt/parser/section/dummy_builder.hpp"

#include "tt/parser/parser_error.hpp"  // for ParserError
#include "tt/parser/section/utils.hpp" // for IsEmptyLine

tt::DummyBuilder::DummyBuilder() noexcept : SectionBuilder("dummy") {}

auto tt::DummyBuilder::ParseLine(const std::string &line) noexcept
    -> tl::expected<void, ParserError> {
    if (IsEmptyLine(line)) {
        return {};
    }

    return make_parser_error<void>(
        ParserError::Type::FoundLineBeforeMainSection,
        "Found line before [main] section");
}
