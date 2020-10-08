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

#include "tt/parser/line/section_line_parser.hpp"

#include <string>

#include "catch2/catch.hpp"

using tt::SectionLineParser;

TEST_CASE("SectionLineParser") {
    SECTION("parse valid section") {
        auto parser = SectionLineParser("[foo]");
        REQUIRE(parser.IsLineValid());
    }

    SECTION("parse invalid section") {
        auto parser = SectionLineParser("foo");
        REQUIRE(!parser.IsLineValid());
    }

    SECTION("parse section not closed") {
        auto parser = SectionLineParser("[foo");
        REQUIRE(!parser.IsLineValid());
    }

    SECTION("garbage after the section is no allowed") {
        auto parser = SectionLineParser("[foo] bar");
        REQUIRE(!parser.IsLineValid());
    }

    SECTION("GetSectionOrDefault returns the valid section") {
        auto parser = SectionLineParser("[foo]");
        REQUIRE(parser.GetSectionOrDefault("bar") == "foo");
    }

    SECTION("GetSectionOrDefault returns the default section") {
        auto parser = SectionLineParser("foo");
        const auto *_default = "bar";
        REQUIRE(parser.GetSectionOrDefault(_default) == _default);
    }
}
