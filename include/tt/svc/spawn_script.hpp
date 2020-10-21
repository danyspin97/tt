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
#include "tt/svc/spawn_supervise.hpp"
#include "tt/svc/types.hpp"

namespace tt {

class Timeout;

class SpawnScript {
public:
    explicit SpawnScript(const std::string &service_name, const Script &script,
                         const Environment &environment, Logger logger);
    virtual ~SpawnScript() = default;

    [[nodiscard]] auto service_name() const -> const std::string &;

    void Kill(Timeout timeout);
    virtual void Signal(int signum);
    auto Spawn() -> ScriptStatus;

protected:
    void ExecuteScript();
    [[nodiscard]] auto GetEnviromentFromScript() const
        -> std::vector<const char *>;
    auto GetExitStatus() -> int;
    virtual auto HasExited() -> bool;
    void SetupUidGid();
    auto TrySpawn(Timeout timeout) -> ScriptStatus;

private:
    const std::string &service_name_;
    Script script_;
    const Environment &environment_;

    std::unique_ptr<TinyProcessLib::Process> process_;
    int exit_status_ = -1;
    ScriptLogger logger_;
};

} // namespace tt
