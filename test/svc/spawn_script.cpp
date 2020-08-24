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

#include "tt/svc/spawn_script.hpp"

#include "catch2/catch.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <unistd.h>

TEST_CASE("SpawnScript") {
    spdlog::set_level(spdlog::level::debug);
    spdlog::stdout_color_mt("oneshot");

    SECTION("Spawn sucessfull script") {
        tt::Environment env;
        tt::Script script{tt::Script::Type::Bash, "exit 0"};
        tt::SpawnScript spawn_script("test-script", script, env,
                                     [](std::string) {});
        tt::ScriptStatus status = spawn_script.Spawn();
        CHECK(status == tt::ScriptStatus::Success);
    }
}
