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
#include "tt/parser/section/utils.hpp"
#include "catch2/catch.hpp"

using tt::IsEmptyLine;
using tt::SetThrowsIfNotEmpty;

TEST_CASE("SetThrowsIfNotEmpty on a non empty string") {
    std::string mystring = "oldValue";
    REQUIRE_THROWS(SetThrowsIfNotEmpty(mystring, "newValue"));
}

TEST_CASE("SetThrowsIfNotEmpty on a empty string") {
    std::string mystring;
    const auto *newValue = "newValue";
    SetThrowsIfNotEmpty(mystring, newValue);
    REQUIRE(mystring == newValue);
}

TEST_CASE("Run IsEmptyLine on empty lines") {
    CHECK(IsEmptyLine(""));
    CHECK(IsEmptyLine(" "));
    CHECK(IsEmptyLine("# "));
    CHECK(IsEmptyLine(" # "));
}

TEST_CASE("Run IsEmptyLine on non-empty lines") {
    CHECK_FALSE(IsEmptyLine("foo = bar"));
    CHECK_FALSE(IsEmptyLine("  foo = bar"));
}
