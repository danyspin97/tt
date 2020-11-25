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

#include "tt/parser/line/key_value_parser.hpp"

#include <string>

#include "catch2/catch.hpp"

#include "tt/parser/parser_error.hpp" // for ParserError

TEST_CASE("parse a value with no spaces") {
    auto result = tt::KeyValueParser::ParseLine("foo=bar");
    REQUIRE(result);
    CHECK(result.value().first == "foo");
    CHECK(result.value().second == "bar");
}

TEST_CASE("parse a value with spaces in it") {
    auto result = tt::KeyValueParser::ParseLine("foo=bar foobar");
    REQUIRE(result);
    CHECK(result.value().first == "foo");
    CHECK(result.value().second == "bar foobar");
}

TEST_CASE("key and value have been trimmed") {
    auto result = tt::KeyValueParser::ParseLine(" foo = bar ");
    REQUIRE(result);
    CHECK(result.value().first == "foo");
    CHECK(result.value().second == "bar");
}

TEST_CASE("parse a not valid line") {
    auto result = tt::KeyValueParser::ParseLine("foobar");
    REQUIRE_FALSE(result.has_value());
}
