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

#include "tt/svc/spawn_supervise.hpp"
#include "tt/svc/types.hpp"

#include "catch2/catch.hpp"

#include <unistd.h>

#include <filesystem>

TEST_CASE("SpawnScript") {
    SECTION("Spawn sucessfull script") {
        std::filesystem::path testfile{"/tmp/tt"};
        if (std::filesystem::exists(testfile)) {
            std::filesystem::remove(testfile);
        }

        tt::PipeFd pipe1;
        tt::PipeFd pipe2;
        pipe(pipe1.data());
        pipe(pipe2.data());

        tt::PipeFd supervisor_fd{pipe1.at(0), pipe2.at(1)};
        // tt::PipeFd process_fd{pipe2.at(0), pipe1.at(1)};
        tt::SpawnSupervise supervise(supervisor_fd);

        if (int pid = fork(); pid == 0) {
            std::vector<char *> args{};
            std::vector<const char *> env{};
            env.push_back("PATH=/bin:/usr/bin");
            env.push_back("SHELL=/bin/bash");
            env.push_back(0);
            args.push_back(const_cast<char *>("touch"));
            args.push_back(const_cast<char *>(testfile.c_str()));
            supervise.Spawn(args, env);
        } else {
            // Wait for the file to be created
            sleep(1);
            CHECK(std::filesystem::exists(testfile));
        }
    }
}
