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

#include "tt/svc/timeout.hpp"
#include "tt/svc/utils.hpp"

tt::SpawnScript::SpawnScript(const std::string &service_name,
                             const Script &script,
                             const Environment &environment)
    : service_name_(service_name), script_(script), environment_(environment) {}

auto tt::SpawnScript::Spawn() -> ScriptStatus {
    auto max_death = script_.max_death();
    decltype(max_death) time_tried = 0;
    while (time_tried != max_death) {
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
    proc_ = redi::ipstream(command,
                           redi::pstreams::pstdout | redi::pstreams::pstderr);
    std::string line;
    std::array<bool, 2> finished = {false, false};
    uint_fast8_t count = 0;
    uint_fast8_t max_lines = 10;
    while (!finished[0] || !finished[1]) {
        if (!finished[0]) {
            while (getline_async(proc_.err(), line) && count != max_lines) {
                spdlog::get("oneshot")->error("{}: {}", service_name_, line);
                count++;
            }
            if (proc_.err().eof()) {
                finished[0] = true;
                if (!finished[1]) {
                    proc_.clear();
                }
            }
        }
        if (!finished[1]) {
            while (getline_async(proc_.out(), line) && count != max_lines) {
                spdlog::get("oneshot")->info("{}: {}", service_name_, line);
                count++;
            }
            if (proc_.out().eof()) {
                finished[1] = true;
                if (!finished[0]) {
                    proc_.clear();
                }
            }
        }
        if (timeout.TimedOut()) {
            break;
        }
        if (count == 0) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(script_.timeout() / 100));
        }
        count = 0;
    }
    proc_.close();
    if (HasExited()) {
        if (proc_.rdbuf()->status() != 0) {
            return ScriptStatus::Failure;
        }
        return ScriptStatus::Success;
    }
    Kill();

    return ScriptStatus::Failure;
}

void tt::SpawnScript::Kill() {
    Signal(static_cast<int>(script_.down_signal()));
    if (!HasExited()) {
        auto wait = script_.timeout_kill() / 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(wait));
        if (!HasExited()) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(script_.timeout_kill() - wait));
            if (!HasExited()) {
                Signal(static_cast<int>(Signal::kSigKill));
            }
        }
    }
}

void tt::SpawnScript::Signal(int signum) { proc_.rdbuf()->kill(signum); }

auto tt::SpawnScript::HasExited() -> bool { return proc_.rdbuf()->exited(); }

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
