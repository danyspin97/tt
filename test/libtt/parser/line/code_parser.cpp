// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#include "libtt/parser/line/code_parser.hpp"

#include <string>
#include "catch2/catch.hpp"

using std::string;
using tt::CodeParser;

TEST_CASE("CodeParser parse code") {
    string newline = "\n";
    // Start parsing
    auto parser = CodeParser();
    string token = "execute =(";
    CHECK(parser.StartParsing(token));
    CHECK(parser.IsParsing());
    CHECK(parser.key() == "execute");

    // Continue parsing
    string line1 = "foo";
    parser.ParseLine(line1);
    CHECK(parser.IsParsing());
    // Do not get code until the parser has finished
    REQUIRE_THROWS(parser.code());

    string line2 = "bar";
    parser.ParseLine(line2);
    CHECK(parser.IsParsing());

    // Cannot parse a new value until the previous has been entirely parsed
    REQUIRE_THROWS(parser.StartParsing(token));

    // End parsing
    parser.ParseLine(")");
    CHECK(parser.code() == line1 + newline + line2 + newline);
    CHECK_FALSE(parser.IsParsing());
}

TEST_CASE("CodeParser parses invalid lines") {
    auto parser = CodeParser();

    SECTION("no equal token") {
        CHECK_FALSE(parser.StartParsing("foo"));
        CHECK_FALSE(parser.IsParsing());
    }

    SECTION("no parenthesis") {
        CHECK_FALSE(parser.StartParsing("execute = "));
        CHECK_FALSE(parser.IsParsing());
    }

    SECTION("value after parenthesis") {
        CHECK_FALSE(parser.StartParsing("execute =( foo "));
        CHECK_FALSE(parser.IsParsing());
    }
}
