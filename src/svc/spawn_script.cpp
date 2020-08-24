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

#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <future>

#include "spdlog/spdlog.h"

#include "pstream.h"

tt::SpawnScript::SpawnScript(const std::string &service_name,
                             const Script &script,
                             const Environment &environment,
                             OnSuccessCallback &&on_success)
    : service_name_(service_name), script_(script), environment_(environment),
      on_success_(std::move(on_success)) {}

auto tt::SpawnScript::Spawn() -> ScriptStatus {
    auto max_death = script_.max_death();
    decltype(max_death) time_tried = 0;
    while (time_tried != script_.max_death()) {
        // This method is already running in a different thread than tt-svc
        // Use std::launch::deferred to not use a third thread
        std::future<ScriptStatus> future =
            std::async(std::launch::deferred, &SpawnScript::TrySpawn, this,
                       Timeout(std::chrono::milliseconds(script_.timeout())));

        if (future.get() == ScriptStatus::Success) {
            return ScriptStatus::Success;
        }
        time_tried++;
    }
    return ScriptStatus::Failure;
}

auto tt::SpawnScript::TrySpawn(Timeout timeout) -> ScriptStatus {
    std::string command{script_.execute()};
    redi::ipstream proc(command,
                        redi::pstreams::pstdout | redi::pstreams::pstderr);
    std::string line;
    std::array<bool, 2> finished = {false, false};
    uint8_t count = 0;
    uint8_t max_lines = 10;
    while (!finished[0] || !finished[1]) {
        if (!finished[0]) {
            while (std::getline(proc.err(), line) && count != max_lines) {
                spdlog::get("oneshot")->error("{}: {}", service_name_, line);
                count++;
            }
            count = 0;
            if (proc.err().eof()) {
                finished[0] = true;
                if (!finished[1]) {
                    proc.clear();
                }
            }
        }
        if (timeout.TimedOut()) {
            break;
        }
        if (!finished[1]) {
            while (std::getline(proc.out(), line) && count != max_lines) {
                spdlog::get("oneshot")->info("{}: {}", service_name_, line);
                count++;
            }
            count = 0;
            if (proc.out().eof()) {
                finished[1] = true;
                if (!finished[0]) {
                    proc.clear();
                }
            }
        }
        if (timeout.TimedOut()) {
            break;
        }
    }
    proc.close();
    if (proc.rdbuf()->exited()) {
        if (proc.rdbuf()->status() != 0) {
            return ScriptStatus::Failure;
        }
        return ScriptStatus::Success;
    }
    // TODO: Implement kill

    return ScriptStatus::Failure;
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

void tt::SpawnScript::SetupUidGid() {
    // TODO: implement
}
