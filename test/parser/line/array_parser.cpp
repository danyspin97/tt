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

#include "tt/parser/line/array_parser.hpp"

#include "catch2/catch.hpp"

TEST_CASE("ArrayParser") {
    auto parser = tt::ArrayParser();

    SECTION("Parse valid values") {
        REQUIRE(parser.StartParsing("foo=("));
        CHECK(parser.IsParsing());
        CHECK(parser.key() == "foo");

        parser.ParseLine("   bar)");
        CHECK_FALSE(parser.IsParsing());
        auto values = parser.values();
        CHECK(values == std::vector<std::string>{"bar"});

        parser.Reset();
        CHECK_FALSE(parser.IsParsing());
        CHECK(parser.key() == "");
        CHECK(parser.values().size() == 0);

        // Check values returned was a copy
        CHECK(values.size() == 1);
    }
    SECTION("Parse a one-line") {
        REQUIRE(parser.StartParsing("foo = (bar foobar)"));
        CHECK_FALSE(parser.IsParsing());
        CHECK(parser.values() == std::vector<std::string>{"bar", "foobar"});
    }

    SECTION("Parse ending token with no values") {
        REQUIRE(parser.StartParsing("foo = (bar"));
        CHECK(parser.IsParsing());

        parser.ParseLine(")");
        CHECK(parser.values() == std::vector<std::string>{"bar"});
        CHECK_FALSE(parser.IsParsing());
    }

    SECTION("Parse invalid line") {
        CHECK_FALSE(parser.StartParsing("foo"));
        CHECK_FALSE(parser.IsParsing());

        CHECK_FALSE(parser.StartParsing("foo = bar("));
    }

    SECTION("Parse line with ending token and now values") {
        CHECK_THROWS(parser.StartParsing("foo = ()"));
    }
}
