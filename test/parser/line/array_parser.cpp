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

#include "tt/parser/line/array_parser.hpp"

#include "catch2/catch.hpp"

#include "tt/parser/define.hpp"
#include "tt/parser/parser_error.hpp" // for ParserError

using std::string;

using tt::kArrayCloseToken;
using tt::kArrayOpenToken;

TEST_CASE("ArrayParser") {
    auto parser = tt::ArrayParser();

    SECTION("Parse valid values") {
        auto ret = parser.StartParsing("foo=" + string{kArrayOpenToken});
        REQUIRE(ret.has_value());
        CHECK(ret.value());
        CHECK(parser.IsParsing());
        CHECK(parser.key() == "foo");

        auto ret2 = parser.ParseLine("   bar" + string{kArrayCloseToken});
        REQUIRE(ret2.has_value());
        CHECK_FALSE(parser.IsParsing());
        auto values = parser.values();
        CHECK(values == std::vector<std::string>{"bar"});

        parser.Reset();
        CHECK_FALSE(parser.IsParsing());
        CHECK(parser.key().empty());
        CHECK(parser.values().empty());

        // Check values returned was a copy
        CHECK(values.size() == 1);
    }

    SECTION("Parse a one-line") {
        const string line = "foo = " + string{kArrayOpenToken} + "bar foobar" +
                            string{kArrayCloseToken};
        auto ret = parser.StartParsing(line);
        REQUIRE(ret.has_value());
        CHECK(ret.value());
        CHECK_FALSE(parser.IsParsing());
        CHECK(parser.values() == std::vector<std::string>{"bar", "foobar"});
    }

    SECTION("Parse ending token with no values") {
        auto ret =
            parser.StartParsing("foo = " + string{kArrayOpenToken} + "bar");
        REQUIRE(ret.has_value());
        REQUIRE(ret.value());
        CHECK(parser.IsParsing());

        auto parser_res = parser.ParseLine(string{kArrayCloseToken});
        REQUIRE(parser_res.has_value());
        CHECK(parser.values() == std::vector<std::string>{"bar"});
        CHECK_FALSE(parser.IsParsing());
    }

    SECTION("Parse invalid line") {
        auto ret = parser.StartParsing("foo");
        CHECK(ret.has_value());
        CHECK_FALSE(ret.value());
        CHECK_FALSE(parser.IsParsing());

        auto ret2 = parser.StartParsing("foo = bar" + string{kArrayOpenToken});
        REQUIRE(ret.has_value());
        CHECK_FALSE(ret.value());
    }

    SECTION("Parse line with ending token and no values") {
        auto ret = parser.StartParsing("foo = " + string{kArrayOpenToken} +
                                       string{kArrayCloseToken});
        CHECK_FALSE(ret.has_value());
    }
}
