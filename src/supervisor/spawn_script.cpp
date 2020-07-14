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

#include "tt/supervisor/spawn_script.hpp"

#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>

#include "spdlog/spdlog.h"

#include "tt/supervisor/supervisor.hpp"

tt::SpawnScript::SpawnScript(const Script &script,
                             const Environment &environment,
                             OnSuccessCallback &&on_success)
    : script_(script), environment_(environment),
      on_success_(std::move(on_success)) {}

auto tt::SpawnScript::Spawn() -> ScriptStatus {
    auto max_death = script_.max_death();
    decltype(max_death) time_tried = 0;
    while (time_tried != script_.max_death()) {
        if (TrySpawn() == ScriptStatus::Success) {
            return ScriptStatus::Success;
        }
        time_tried++;
    }
    return ScriptStatus::Failure;
}

auto tt::SpawnScript::TrySpawn() -> ScriptStatus {
    if (int pid = fork(); pid == 0) {
        PipeFd supervisor_fd = SetupProcessFdAndGetSupervisorFd();
        SetupUidGid();
        std::vector<char *> args{};
        args.push_back(const_cast<char *>("sh"));
        args.push_back(const_cast<char *>("-c"));
        std::string execute = script_.execute();
        std::vector<char> cexecute(execute.c_str(),
                                   execute.c_str() + execute.size() + 1);
        args.push_back(cexecute.data());
        auto environment_vec = environment_.Vector();
        std::vector<const char *> environment_vec_cstr{environment_vec.size()};
        std::for_each(environment_vec.begin(), environment_vec.end(),
                      [&environment_vec_cstr](const std::string &key_value) {
                          environment_vec_cstr.push_back(key_value.c_str());
                      });
        Supervisor supervisor(supervisor_fd);
        supervisor.Supervise(args, environment_vec_cstr);
    } else {
        std::array<pollfd, 1> pollfds{process_fd_.at(0), POLLIN | POLLRDHUP, 0};
        if (poll(pollfds.data(), 1, script_.timeout()) != 1) {
            spdlog::critical("Failed to run poll()");
        }
        if (pollfds[0].revents & (POLLERR | POLLNVAL | POLLRDHUP | POLLHUP)) {
            return ScriptStatus::Failure;
        }
        if (pollfds[0].revents & POLLIN) {
            return GetCurrentScriptStatusFromProcessFd();
        }
        // TODO: Handle case with timeout and kill the process
        return ScriptStatus::Failure;
    }
    assert(false);
}

auto tt::SpawnScript::GetCurrentScriptStatusFromProcessFd() -> ScriptStatus {
    siginfo_t scriptinfo = {};
    if (read(process_fd_.at(0), &scriptinfo, sizeof(scriptinfo)) < 0) {
        spdlog::critical("Failed to read from process_fd");
        return ScriptStatus::Failure;
    }
    if (scriptinfo.si_status == 0) {
        return ScriptStatus::Success;
    }
    return ScriptStatus::Failure;
}

auto tt::SpawnScript::SetupProcessFdAndGetSupervisorFd() -> PipeFd {
    PipeFd pipe1;
    PipeFd pipe2;
    pipe(pipe1.data());
    pipe(pipe2.data());

    process_fd_.at(0) = pipe2.at(0);
    process_fd_.at(1) = pipe1.at(1);

    PipeFd supervisor_fd{pipe1.at(0), pipe2.at(1)};
    return supervisor_fd;
}

void tt::SpawnScript::SetupUidGid() {
    // TODO: implement
}
