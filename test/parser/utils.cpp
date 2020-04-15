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

#include "catch2/catch.hpp"

#include "tt/parser/utils.hpp"
#include "tt/parser/exception.hpp"

using tt::BooleanParseException;
using tt::ParseBoolean;

TEST_CASE("ParseBoolean") {
    SECTION("Parse valid values") {
        CHECK(ParseBoolean("yes"));
        CHECK_FALSE(ParseBoolean("no"));
    }

    SECTION("Parse invalid values") {
        CHECK_THROWS_AS(ParseBoolean("foo"), BooleanParseException);
    }
}
