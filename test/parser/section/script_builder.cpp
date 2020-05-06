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

#include "tt/parser/section/script_builder.hpp"

#include "catch2/catch.hpp"

#include "tt/parser/section/utils.hpp"

using tt::Environment;
using tt::Script;
using tt::ScriptBuilder;
using tt::TestBuilderWithFile;

TEST_CASE("ScriptBuilder") {
    Environment e;
    auto builder = ScriptBuilder(e, "test");
    SECTION("Parse valid script section") {
        TestBuilderWithFile(builder, "../test/data/script_section");
        Script s = builder.script();

        CHECK(s.user().value() == "dbus");
        CHECK(s.group().value() == "dbus");
    }

    SECTION("Parse invalid line") {
        REQUIRE_THROWS(builder.ParseLine("invalid"));
    }
}
