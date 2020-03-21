// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#include "libtt/parser/line/section_line_parser.hpp"

#include <string>

#include "catch2/catch.hpp"

using tt::SectionLineParser;

TEST_CASE("SectionLineParser") {
    SECTION("parse valid section") {
        auto parser = SectionLineParser("[foo]");
        REQUIRE(parser.line_valid());
    }

    SECTION("parse invalid section") {
        auto parser = SectionLineParser("foo");
        REQUIRE(!parser.line_valid());
    }

    SECTION("GetSectionOrDefault returns the valid section") {
        auto parser = SectionLineParser("[foo]");
        REQUIRE(parser.GetSectionOrDefault("bar") == "foo");
    }

    SECTION("GetSectionOrDefault returns the default section") {
        auto parser = SectionLineParser("foo");
        auto _default = "bar";
        REQUIRE(parser.GetSectionOrDefault(_default) == _default);
    }
}
