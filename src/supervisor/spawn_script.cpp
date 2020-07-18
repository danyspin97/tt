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

std::mutex tt::SpawnScript::fork_mutex_;

tt::SpawnScript::SpawnScript(const Script &script,
                             const Environment &environment,
                             OnSuccessCallback &&on_success)
    : script_(script), environment_(environment),
      on_success_(std::move(on_success)) {}

auto tt::SpawnScript::Spawn() -> ScriptStatus {
    auto max_death = script_.max_death();
    decltype(max_death) time_tried = 0;
    while (time_tried != script_.max_death()) {
        if (status_ == ScriptStatus::Success) {
            return ScriptStatus::Success;
        }
        time_tried++;
    }
    return ScriptStatus::Failure;
}

void tt::SpawnScript::TrySpawn() {
    SetupFds();
    LockForkMutex();
    AdjustSupervisionFdFlags();
    if (int pid = fork(); pid == 0) {
        SetupUidGid();
        Supervisor supervisor(supervisor_fd_);
        supervisor.Supervise(GetSupervisorArgs(), GetEnviromentFromScript());
    } else {
        UnlockForkMutex();
        WaitOnStatus();
    }
    assert(false);
}

void tt::SpawnScript::WaitOnStatus() {
    std::array<pollfd, 1> pollfds{process_fd_.at(0), POLLIN | POLLRDHUP, 0};
    if (poll(pollfds.data(), 1, script_.timeout()) != 1) {
        spdlog::critical("Failed to run poll()");
    }
    if (HasConnectionHungUp(pollfds[0].revents)) {
        spdlog::warn("Pipe error");
    }
    if (HasReceivedUpdate(pollfds[0].revents)) {
        ReadStatusFromProcessFd();
    }
    // TODO: Handle case with timeout and kill the process
}

void tt::SpawnScript::ReadStatusFromProcessFd() {
    siginfo_t scriptinfo = {};
    if (read(process_fd_.at(0), &scriptinfo, sizeof(scriptinfo)) < 0) {
        spdlog::critical("Failed to read from process_fd");
    }
    if (scriptinfo.si_status == 0) {
        status_ = ScriptStatus::Success;
    }
}

auto tt::SpawnScript::GetSupervisorArgs() -> std::vector<char *> {
    std::vector<char *> args{};
    args.push_back(const_cast<char *>("sh"));
    args.push_back(const_cast<char *>("-c"));
    std::string execute = script_.execute();
    std::vector<char> cexecute(execute.c_str(),
                               execute.c_str() + execute.size() + 1);
    args.push_back(cexecute.data());
    return args;
}

auto tt::SpawnScript::GetEnviromentFromScript() -> std::vector<const char *> {
    auto environment_vec = environment_.Vector();
    std::vector<const char *> environment_vec_cstr{environment_vec.size()};
    std::for_each(environment_vec.begin(), environment_vec.end(),
                  [&environment_vec_cstr](const std::string &key_value) {
                      environment_vec_cstr.push_back(key_value.c_str());
                  });
    return environment_vec_cstr;
}

void tt::SpawnScript::SetupFds() {
    PipeFd first;
    PipeFd second;
    pipe2(first.data(), O_CLOEXEC);
    pipe2(second.data(), O_CLOEXEC);

    process_fd_.at(0) = second.at(0);
    process_fd_.at(1) = first.at(1);

    supervisor_fd_.at(0) = first.at(0);
    supervisor_fd_.at(1) = second.at(1);
}

void tt::SpawnScript::SetupUidGid() {
    // TODO: implement
}

void tt::SpawnScript::AdjustSupervisionFdFlags() {
    for (auto &fd : supervisor_fd_) {
        int flags;
        if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
            spdlog::critical("Error on F_GETFL");
        } else {
            flags &= ~O_CLOEXEC;
            if (fcntl(fd, F_SETFL, flags) < 0) {
                spdlog::critical("Error on F_SETFL");
            }
        }
    }
}

auto tt::SpawnScript::HasConnectionHungUp(short revents) -> bool {
    return revents & (POLLERR | POLLNVAL | POLLHUP);
}

auto tt::SpawnScript::HasReceivedUpdate(short revents) -> bool {
    return revents & POLLIN;
}

void tt::SpawnScript::LockForkMutex() { fork_mutex_.lock(); }

void tt::SpawnScript::UnlockForkMutex() { fork_mutex_.unlock(); }
