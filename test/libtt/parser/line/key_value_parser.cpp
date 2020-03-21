// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#include "libtt/parser/line/key_value_parser.hpp"

#include <string>

#include "catch2/catch.hpp"

using tt::KeyValueParser;

TEST_CASE("parse a value with no spaces") {
    std::string line = "foo=bar";
    auto parser = KeyValueParser(line, false);
    CHECK(parser.isLineValid());
    CHECK(parser.getKey() == "foo");
    CHECK(parser.getValue() == "bar");
}

TEST_CASE("parse a value with spaces in it") {
    auto parser = KeyValueParser("foo=bar foobar");
    CHECK(parser.isLineValid());
    CHECK(parser.getKey() == "foo");
    CHECK(parser.getValue() == "bar foobar");
}

TEST_CASE("key and value have been trimmed") {
    auto parser = KeyValueParser(" foo = bar ");
    CHECK(parser.isLineValid());
    CHECK(parser.getKey() == "foo");
    CHECK(parser.getValue() == "bar");
}

TEST_CASE("parse a not valid line") {
    auto parser = KeyValueParser("foobar");
    REQUIRE_FALSE(parser.isLineValid());
}

TEST_CASE("parse a not valid line and throws an exception") {
    REQUIRE_THROWS(KeyValueParser("foo", true));
}
