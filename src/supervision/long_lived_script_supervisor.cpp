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

#include <array>    // for array
#include <chrono>   // for milliseconds
#include <cstdint>  // for uint_fast32_t
#include <optional> // for optional
#include <poll.h>   // for poll, POLLIN, pollfd
#include <thread>   // for sleep_for
#include <unistd.h> // for dup, close, dup2, pipe
#include <utility>  // for move

namespace tt {
class Environment;
} // namespace tt

tt::LongLivedScriptSupervisor::LongLivedScriptSupervisor(
    const std::string &service_name, const LongLivedScript &script,
    const Environment &environment, const ScriptLogger &logger)
    : ScriptSupervisor(service_name, script, environment, logger),
      long_lived_script_(script) {}

auto tt::LongLivedScriptSupervisor::ExecuteScript() -> ScriptStatus {
    if (long_lived_script_.notify()) {
        SetupNotifyFd();
    }
    LaunchProcess();
    if (long_lived_script_.notify()) {
        return ListenOnNotifyFd();
    }
    std::this_thread::sleep_for(
        std::chrono::milliseconds(long_lived_script_.timeout()));
    return ScriptStatus::Success;
}

auto tt::LongLivedScriptSupervisor::ListenOnNotifyFd() const -> ScriptStatus {
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
