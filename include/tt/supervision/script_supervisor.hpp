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

#pragma once

#include <chrono>
#include <memory>

#include "process.hpp"

#include "tt/data/service.hpp"
#include "tt/log/script_logger.hpp"
#include "tt/supervision/spawn_supervise.hpp"
#include "tt/supervision/types.hpp"

namespace tt {

class Timeout;

class ScriptSupervisor {
public:
    explicit ScriptSupervisor(const std::string &service_name,
                              const Script &script,
                              const Environment &environment,
                              ScriptLogger logger);
    virtual ~ScriptSupervisor() = default;

    [[nodiscard]] auto service_name() const -> const std::string &;

    // Execute the script and returns ScriptStatus::Success if it exited
    // successfully if it exits with non 0 status code or it takes more than
    // timeout milliseconds to run, returns ScriptStatus::Failure
    auto Execute() -> ScriptStatus;
    // send script.down_signal to the process and if it hasn't exited
    // in script.timeout_kill milliseconds, send SIGKILL
    void Kill();
    // Send signum to the process
    void Signal(int signum);

protected:
    [[nodiscard]] auto ExecuteUntilTimeout(Timeout timeout) -> ScriptStatus;
    [[nodiscard]] auto GetEnviromentFromScript() const
        -> std::vector<const char *>;
    // Get the exit status of the process, block if it hasn't exited
    [[nodiscard]] auto GetExitStatus() -> int;
    // Has the process exited? Non-blocking call
    [[nodiscard]] auto HasExited() -> bool;
    // Create a new instance of TinyProcessLib::Process that will fork and
    // execute the script using execv
    void LaunchProcess();

private:
    const std::string &service_name_;
    Script script_;
    const Environment &environment_;

    std::unique_ptr<TinyProcessLib::Process> process_;
    int exit_status_ = -1;
    ScriptLogger logger_;
};

} // namespace tt
