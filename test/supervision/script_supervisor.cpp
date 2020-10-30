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

#include "tt/supervision/script_supervisor.hpp"

#include <chrono>     // for milliseconds
#include <filesystem> // for exists, remove, file_size
#include <thread>     // for sleep_for

#include "catch2/catch.hpp"                  // for operator""_catch_sr
#include "spdlog/sinks/stdout_color_sinks.h" // for stdout_color_mt

#include "tt/data/environment.hpp" // for Environment

auto console = spdlog::stdout_color_mt("spawn_script_test");
tt::ScriptLogger logger{console};

TEST_CASE("ScriptSupervisor") {
    tt::Environment env;

    SECTION("Execute sucessfull script") {
        tt::Script script{tt::Script::Type::SH, "true"};
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env,
                                          logger);
        tt::ScriptStatus status = spawn_script.ExecuteScript();
        CHECK(status == tt::ScriptStatus::Success);
    }

    SECTION("Execute failing script") {
        tt::Script script{tt::Script::Type::SH, "false"};
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env,
                                          logger);
        tt::ScriptStatus status = spawn_script.ExecuteScript();
        CHECK(status == tt::ScriptStatus::Failure);
    }

    SECTION("Execute script within timeout") {
        tt::Script script{tt::Script::Type::SH, "sleep 1"};
        script.timeout(2000);
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env,
                                          logger);
        tt::ScriptStatus status = spawn_script.ExecuteScript();
        CHECK(status == tt::ScriptStatus::Success);
    }

    SECTION("Execute script that exceed timeout") {
        tt::Script script{tt::Script::Type::SH, "sleep 10"};
        script.timeout(50);
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env,
                                          logger);
        tt::ScriptStatus status = spawn_script.ExecuteScript();
        CHECK(status == tt::ScriptStatus::Failure);
    }

    SECTION("Execute script within timeout and fail") {
        tt::Script script{tt::Script::Type::SH, "sleep 1 && false"};
        script.timeout(2000);

        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env,
                                          logger);
        tt::ScriptStatus status = spawn_script.ExecuteScript();
        CHECK(status == tt::ScriptStatus::Failure);
    }

    SECTION("Check time tried") {
        // Cleanup previous test
        std::filesystem::path testfile{"TIME_TRIED"};
        if (std::filesystem::exists(testfile)) {
            std::filesystem::remove(testfile);
        }

        constexpr auto time_to_try = 3;
        const auto *command = "echo 1 >> TIME_TRIED ; exit 1";
        tt::Script script{tt::Script::Type::SH, command};
        script.max_death(time_to_try);

        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env,
                                          logger);

        tt::ScriptStatus status = spawn_script.ExecuteScript();
        CHECK(status == tt::ScriptStatus::Failure);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        REQUIRE(std::filesystem::exists(testfile));
        const auto size = std::filesystem::file_size(testfile);
        CHECK(size == time_to_try * 2);
    }

    SECTION("Check process is killed") {
        // Cleanup previous test
        std::filesystem::path testfile{"PROCESS_EXISTS"};
        if (std::filesystem::exists(testfile)) {
            std::filesystem::remove(testfile);
        }

        const auto *command = "sleep 1 && touch PROCESS_EXISTS";
        tt::Script script{tt::Script::Type::SH, command};
        script.timeout(50);
        script.max_death(1);

        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env,
                                          logger);
        tt::ScriptStatus status = spawn_script.ExecuteScript();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        CHECK(status == tt::ScriptStatus::Failure);
        CHECK_FALSE(std::filesystem::exists(testfile));
    }

    SECTION("Execute sucessfull path") {
        tt::Script script{tt::Script::Type::Path, "sleep 0"};
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env,
                                          logger);
        tt::ScriptStatus status = spawn_script.ExecuteScript();
        CHECK(status == tt::ScriptStatus::Success);
    }

    SECTION("Execute sucessfull sleep as path") {
        tt::Script script{tt::Script::Type::Path, "sleep 2"};
        script.timeout(5000);
        tt::ScriptSupervisor spawn_script("test-path-sleep", script, env,
                                          logger);
        tt::ScriptStatus status = spawn_script.ExecuteScript();
        CHECK(status == tt::ScriptStatus::Success);
    }
}
