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

#include <chrono>
#include <filesystem>
#include <thread>

bool logger_set = false;

TEST_CASE("SpawnScript") {
    if (!logger_set) {
        spdlog::set_level(spdlog::level::debug);
        spdlog::stdout_color_mt("oneshot");
        logger_set = true;
    }
    tt::Environment env;

    SECTION("Spawn sucessfull script") {
        tt::Script script{tt::Script::Type::Bash, "exit 0"};
        tt::SpawnScript spawn_script("test-script", script, env);
        tt::ScriptStatus status = spawn_script.Spawn();
        CHECK(status == tt::ScriptStatus::Success);
    }

    SECTION("Spawn failing script") {
        tt::Script script{tt::Script::Type::Bash, "exit 1"};
        tt::SpawnScript spawn_script("failing-script", script, env);
        tt::ScriptStatus status = spawn_script.Spawn();
        CHECK(status == tt::ScriptStatus::Failure);
    }

    SECTION("Spawn script within timeout") {
        tt::Script script{tt::Script::Type::Bash, "sleep 1"};
        script.timeout(2000);
        tt::SpawnScript spawn_script("timeout-success", script, env);
        tt::ScriptStatus status = spawn_script.Spawn();
        CHECK(status == tt::ScriptStatus::Success);
    }

    SECTION("Spawn script that exceed timeout") {
        tt::Script script{tt::Script::Type::Bash, "sleep 10"};
        script.timeout(50);
        tt::SpawnScript spawn_script("timeout-fail", script, env);
        tt::ScriptStatus status = spawn_script.Spawn();
        CHECK(status == tt::ScriptStatus::Failure);
    }

    SECTION("Spawn script within timeout and fail") {
        tt::Script script{tt::Script::Type::Bash, "sleep 1 && exit 1"};
        script.timeout(2000);
        tt::SpawnScript spawn_script("timeout-fail", script, env);
        tt::ScriptStatus status = spawn_script.Spawn();
        CHECK(status == tt::ScriptStatus::Failure);
    }

    SECTION("Check time tried") {
        std::filesystem::path testfile{"TIME_TRIED"};
        if (std::filesystem::exists(testfile)) {
            std::filesystem::remove(testfile);
        }
        constexpr auto time_to_try = 3;
        auto command = "echo 1 >> TIME_TRIED ; exit 1";
        tt::Script script{tt::Script::Type::Bash, command};
        script.max_death(time_to_try);
        tt::SpawnScript spawn_script("time-tried", script, env);
        tt::ScriptStatus status = spawn_script.Spawn();
        CHECK(status == tt::ScriptStatus::Failure);
        REQUIRE(std::filesystem::exists(testfile));
        const auto size = std::filesystem::file_size(testfile);
        CHECK(size == time_to_try * 2);
    }

    SECTION("Check process is killed") {
        std::filesystem::path testfile{"PROCESS_EXISTS"};
        if (std::filesystem::exists(testfile)) {
            std::filesystem::remove(testfile);
        }
        auto command = "sleep 1 && touch PROCESS_EXISTS";
        tt::Script script{tt::Script::Type::Bash, command};
        script.timeout(50);
        script.max_death(1);
        tt::SpawnScript spawn_script("timeout-fail", script, env);
        tt::ScriptStatus status = spawn_script.Spawn();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        CHECK(status == tt::ScriptStatus::Failure);
        CHECK_FALSE(std::filesystem::exists(testfile));
    }
}
