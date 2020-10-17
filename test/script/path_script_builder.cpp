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

#include "tt/script/path_script_builder.hpp"

#include "catch2/catch.hpp"

TEST_CASE("PathScriptBuilder") {
    SECTION("Parse unquoted execute") {
        std::string execute = "mkdir /tmp/tt";
        tt::PathScriptBuilder builder{};
        auto script = builder.script(execute, tt::Environment{});
        CHECK(script == std::vector<std::string>{"mkdir", "/tmp/tt"});
    }

    SECTION("Parse unquoted execute with multiple spaces") {
        std::string execute = "mkdir      /tmp/tt";
        tt::PathScriptBuilder builder{};
        auto script = builder.script(execute, tt::Environment{});
        CHECK(script == std::vector<std::string>{"mkdir", "/tmp/tt"});
    }

    SECTION("Parse quoted execute") {
        std::string execute = "mkdir \"/tmp/tt\"";
        tt::PathScriptBuilder builder{};
        auto script = builder.script(execute, tt::Environment{});
        CHECK(script == std::vector<std::string>{"mkdir", "/tmp/tt"});
    }

    SECTION("Parse quoted execute with no spaces") {
        std::string execute = R"(mkdir "/tmp/tt"/mytest)";
        tt::PathScriptBuilder builder{};
        auto script = builder.script(execute, tt::Environment{});
        CHECK(script == std::vector<std::string>{"mkdir", "/tmp/tt/mytest"});
    }

    SECTION("Parse quoted execute with backslash") {
        std::string execute = R"(mkdir \"/tmp/tt\")";
        tt::PathScriptBuilder builder{};
        auto script = builder.script(execute, tt::Environment{});
        CHECK(script == std::vector<std::string>{"mkdir", R"(\"/tmp/tt\")"});
    }
}
