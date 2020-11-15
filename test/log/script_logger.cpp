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

#include "catch2/catch.hpp"

#include <filesystem> // for exists, remove
#include <fstream>    // for basic_istream, ifstream
#include <sstream>    // for stringstream

#include "spdlog/async.h"                 // for async_factory
#include "spdlog/logger.h"                // for logger
#include "spdlog/sinks/basic_file_sink.h" // for basic_logger_mt

namespace tt::test {

// Divide a string by newlines and add [stdout] (or stderr) at the start
auto GetExpectedOutput(bool is_stdout, const std::string& output) {
    std::string type = is_stdout ? "stdout" : "stderr";
    std::stringstream ss{output};
    std::stringstream res{output};
    std::string tmp;

    while (std::getline(ss, tmp, '\n')) {
        res << "[" << type << "] " << tmp << "\n";
    }
    return res.str();
}

void TestOutput(const std::string& test_name, const std::string& echo_args,
                const std::string& output, bool is_stdout) {
    std::string command = std::string{is_stdout ? "" : ">&2 "} + "echo " +
                          echo_args + " " + output;
    // We can only redirect when using bash
    auto script_type =
        is_stdout ? tt::Script::Type::Path : tt::Script::Type::SH;
    tt::Script script{script_type, std::move(command)};
    const auto logfile = std::string{test_name} + ".log";
    if (std::filesystem::exists(logfile)) {
        std::filesystem::remove(logfile);
    }
    auto logger =
        spdlog::basic_logger_mt<spdlog::async_factory>(test_name, logfile);
    logger->set_pattern("%v");
    {
        tt::Environment env;
        tt::ScriptSupervisor spawn_script(test_name, script, env,
                                          tt::ScriptLogger(logger));
        tt::ScriptStatus status = spawn_script.ExecuteScript();
        REQUIRE(status == tt::ScriptStatus::Success);
        REQUIRE(std::filesystem::exists(logfile));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::ifstream ifs(logfile);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    CHECK(content == GetExpectedOutput(is_stdout, output));
}

} // namespace tt::test

TEST_CASE("ScriptLogger") {
    SECTION("Read to stdout") {
        tt::test::TestOutput("test-output-stdout", "", "Read line to stdout",
                             true);
    }

    SECTION("Read to stderr") {
        tt::test::TestOutput("test-output-stderr", "", "Read line to stderr",
                             false);
    }

    SECTION("Read multiple empty lines to stdout") {
        tt::test::TestOutput("test-multiline-empty-stdout", "", "\n\n\n5\n",
                             true);
    }

    SECTION("Read multiple lines to stdout") {
        tt::test::TestOutput("test-multiline-stdout", "", "1\n2\n3\n4\n", true);
    }

    SECTION("Read lines to stdout without ending newline") {
        // -n is recognized as echo argument
        tt::test::TestOutput("test-incomplete-line-stdout", "-n", "0\n1\n2\n3",
                             true);
    }
}

TEST_CASE("ScriptLogger fail", "[!shouldfail]") {
    // Empty lines are lost
    SECTION("Read empty output to stdout", "[!mayfail]") {
        tt::test::TestOutput("test-empty-stdout", "", "\n\n\n\n", true);
    }
}
