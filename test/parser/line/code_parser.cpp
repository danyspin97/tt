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

#include "tt/parser/line/code_parser.hpp"

#include "catch2/catch.hpp"
#include <string>

#include "tt/parser/parser_error.hpp" // for ParserError

using std::string;
using tt::CodeParser;

TEST_CASE("CodeParser parse code") {
    string newline = "\n";
    // Start parsing
    auto parser = CodeParser();
    string token = "execute =(";
    auto ret = parser.StartParsing(token);
    REQUIRE(ret.has_value());
    CHECK(ret.value());
    CHECK(parser.IsParsing());
    CHECK(parser.key() == "execute");

    // Continue parsing
    string line1 = "foo";
    parser.ParseLine(line1);
    CHECK(parser.IsParsing());

    string line2 = "bar";
    parser.ParseLine(line2);
    CHECK(parser.IsParsing());

    // End parsing
    parser.ParseLine(")");
    REQUIRE_FALSE(parser.IsParsing());
    CHECK(parser.code() == line1 + newline + line2);
}

TEST_CASE("CodeParser parses invalid lines") {
    auto parser = CodeParser();

    SECTION("no equal token") {
        auto ret = parser.StartParsing("foo");
        REQUIRE(ret.has_value());
        CHECK_FALSE(ret.value());
        CHECK_FALSE(parser.IsParsing());
    }

    SECTION("no parenthesis") {
        auto ret = parser.StartParsing("execute = ");
        REQUIRE(ret.has_value());
        CHECK_FALSE(ret.value());
        CHECK_FALSE(parser.IsParsing());
    }

    SECTION("value after parenthesis") {
        auto ret = parser.StartParsing("execute =( foo ");
        REQUIRE(ret.has_value());
        CHECK_FALSE(ret.value());
        CHECK_FALSE(parser.IsParsing());
    }

    SECTION("empty key") {
        auto ret = parser.StartParsing("=(");
        REQUIRE_FALSE(ret.has_value());
        CHECK(ret.error().type() == tt::ParserError::Type::EmptyKey);
    }
}
