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

#include "tt/supervision/long_lived_script_supervisor.hpp"

#include <poll.h>
#include <unistd.h>

#include <chrono>
#include <future>

#include "spdlog/spdlog.h"

#include "tt/supervision/supervision_signal_handler.hpp"

tt::LongLivedScriptSupervisor::LongLivedScriptSupervisor(
    const std::string &service_name, const LongLivedScript &script,
    const Environment &environment, ScriptLogger logger)
    : ScriptSupervisor(service_name, script, environment, std::move(logger)),
      long_lived_script_(script) {}

auto tt::LongLivedScriptSupervisor::ExecuteScript() -> ScriptStatus {
    if (long_lived_script_.notify()) {
        SetupNotifyFd();
    }
    LaunchProcess();
    if (long_lived_script_.notify()) {
        return ListenOnNotifyFd();
    }
    waiting_on_startup_ = true;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(long_lived_script_.timeout()));
    waiting_on_startup_ = false;
    bool success_ = true;
    if (success_) {
        return ScriptStatus::Success;
    } else {
        return ScriptStatus::Failure;
    }
}

auto tt::LongLivedScriptSupervisor::ListenOnNotifyFd() -> ScriptStatus {
    short revents = 0;
    struct pollfd fd = {notify_fd_, POLLIN, revents};
    int res = poll(&fd, 1, long_lived_script_.timeout());
    // We got a revents on notify_fd_
    if (res == 1 && (revents | POLLIN) != 0) {
        return ScriptStatus::Success;
    }
    // Timeout or error
    return ScriptStatus::Failure;
}

void tt::LongLivedScriptSupervisor::SetupNotifyFd() {
    std::array<int, 2> fd{};
    pipe(fd.data());
    notify_fd_ = dup(fd[0]);
    close(dup(fd[0]));
    dup2(fd[1], static_cast<int>(long_lived_script_.notify().value()));
}

auto tt::LongLivedScriptSupervisor::HasStarted() const -> bool {
    return waiting_on_startup_;
}
