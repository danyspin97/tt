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

#include <csignal>

#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

#include "process.hpp"

#include "tt/script/script_builder_factory.hpp"
#include "tt/script/shell_script_builder.hpp"
#include "tt/status.hpp"
#include "tt/supervision/timeout.hpp"
#include "tt/supervision/utils.hpp"
#include "tt/path/user_dirs.hpp"

tt::ScriptSupervisor::ScriptSupervisor(const std::string &service_name,
                                       const Script &script,
                                       const Environment &environment,
                                       ScriptLogger logger)
    : service_name_(service_name), script_(script), environment_(environment),
      logger_(std::move(logger)) {}

auto tt::ScriptSupervisor::ExecuteScript() -> ScriptStatus {
    auto max_death = script_.max_death();
    decltype(max_death) time_tried = 0;
    ScriptStatus status = ScriptStatus::Failure;
    do {
        status = ExecuteUntilTimeout(
            Timeout{std::chrono::milliseconds(script_.timeout())});
        time_tried++;
    } while (time_tried != max_death && status != ScriptStatus::Success);
    return status;
}

auto tt::ScriptSupervisor::ExecuteUntilTimeout(Timeout timeout)
    -> ScriptStatus {
    LaunchProcess();
    while (!HasExited() && !timeout.TimedOut()) {
        std::this_thread::yield();
    }
    if (HasExited()) {
        if (GetExitStatus() != 0) {
            return ScriptStatus::Failure;
        }
        return ScriptStatus::Success;
    }

    Kill();
    return ScriptStatus::Failure;
}

void tt::ScriptSupervisor::LaunchProcess() {
    auto builder = ScriptBuilderFactory::GetScriptBuilder(script_.type());
    auto command = builder->script(script_.execute(), environment_);
    process_ = std::make_unique<TinyProcessLib::Process>(
        command, "",
        [this](const char *bytes, size_t size) {
            logger_.Log(ScriptLogger::Type::STDOUT, bytes, size);
        },
        [this](const char *bytes, size_t size) {
            logger_.Log(ScriptLogger::Type::STDERR, bytes, size);
        });
}

void tt::ScriptSupervisor::Kill() {
    Timeout timeout{std::chrono::milliseconds(script_.timeout_kill())};
    Signal(static_cast<int>(script_.down_signal()));
    while (!HasExited() && !timeout.TimedOut()) {
        std::this_thread::yield();
    }
    if (!HasExited()) {
        Signal(static_cast<int>(Signal::kSigKill));
    }
}

void tt::ScriptSupervisor::Signal(int signum) { process_->signal(signum); }

auto tt::ScriptSupervisor::HasExited() -> bool {
    if (exit_status_ != -1) {
        return true;
    }

    int exit_status = 0;
    if (process_->try_get_exit_status(exit_status)) {
        exit_status_ = exit_status;
        return true;
    }

    return false;
}

auto tt::ScriptSupervisor::GetExitStatus() -> int {
    if (exit_status_ != -1) {
        return exit_status_;
    }

    return exit_status_ = process_->get_exit_status();
}

auto tt::ScriptSupervisor::GetEnviromentFromScript() const
    -> std::vector<const char *> {
    auto environment_vec = environment_.Vector();
    std::vector<const char *> environment_vec_cstr{environment_vec.size()};
    std::for_each(environment_vec.begin(), environment_vec.end(),
                  [&environment_vec_cstr](const std::string &key_value) {
                      environment_vec_cstr.push_back(key_value.c_str());
                  });
    return environment_vec_cstr;
}

auto tt::ScriptSupervisor::service_name() const -> const std::string & {
    return service_name_;
}
