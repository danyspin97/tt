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

#include "libtt/parser/line/key_value_parser.hpp"

#include <string>

#include "catch2/catch.hpp"

using tt::KeyValueParser;

TEST_CASE("parse a value with no spaces") {
    std::string line = "foo=bar";
    auto parser = KeyValueParser(line, false);
    CHECK(parser.isLineValid());
    CHECK(parser.key() == "foo");
    CHECK(parser.value() == "bar");
}

TEST_CASE("parse a value with spaces in it") {
    auto parser = KeyValueParser("foo=bar foobar");
    CHECK(parser.isLineValid());
    CHECK(parser.key() == "foo");
    CHECK(parser.value() == "bar foobar");
}

TEST_CASE("key and value have been trimmed") {
    auto parser = KeyValueParser(" foo = bar ");
    CHECK(parser.isLineValid());
    CHECK(parser.key() == "foo");
    CHECK(parser.value() == "bar");
}

TEST_CASE("parse a not valid line") {
    auto parser = KeyValueParser("foobar");
    REQUIRE_FALSE(parser.isLineValid());
}

TEST_CASE("parse a not valid line and throws an exception") {
    REQUIRE_THROWS(KeyValueParser("foo", true));
}
