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

#include "tt/supervisor/spawn_supervise.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <charconv>

#include "spdlog/spdlog.h"

tt::SpawnSupervise::SpawnSupervise(PipeFd supervisor_fd)
    : supervisor_fd_(supervisor_fd) {}

void tt::SpawnSupervise::Spawn(std::vector<char *> script,
                               std::vector<const char *> environment) {
    SetupFds();
    execve("/bin/supervise", const_cast<char **>(GetExecArgs(script).data()),
           const_cast<char *const *>(environment.data()));
    spdlog::critical("An error had happened while running execve: {}",
                     strerror(errno));
}

void tt::SpawnSupervise::SetupFds() {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    // No need to close the fds as they have O_CLOEXEC
    dup2(supervisor_fd_.at(0), 0);
    dup2(supervisor_fd_.at(1), 1);
}

auto tt::SpawnSupervise::GetExecArgs(std::vector<char *> script)
    -> std::vector<char *> {
    std::vector<char *> args{};
    args.push_back(const_cast<char *>("supervise"));
    args.push_back(GetCStrFromInt(0).data());
    args.push_back(GetCStrFromInt(1).data());
    std::for_each(script.begin(), script.end(),
                  [&args](char *arg) { args.push_back(arg); });
    args.push_back(0);
    return args;
}

auto tt::SpawnSupervise::GetCStrFromInt(int num) -> std::array<char, 2> {
    std::array<char, 2> cstr{};
    std::to_chars(cstr.data(), cstr.data() + cstr.size(), num);
    return cstr;
}
