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

#pragma once

#include <chrono>

#include "tt/data/service.hpp"
#include "tt/svc/spawn_supervise.hpp"
#include "tt/svc/types.hpp"

namespace tt {

class SpawnScript {
public:
    explicit SpawnScript(const std::string &service_name, const Script &script,
                         const Environment &environment,
                         OnSuccessCallback &&on_success);
    virtual ~SpawnScript() = default;

    auto Spawn() -> ScriptStatus;

    struct Timeout {
        std::chrono::steady_clock::time_point start =
            std::chrono::steady_clock::now();
        std::chrono::milliseconds timeout;
        Timeout(std::chrono::milliseconds t) : timeout(t) {}

        bool TimedOut() {
            return start + timeout < std::chrono::steady_clock::now();
        }
    };

protected:
    auto GetSupervisorArgs() -> std::vector<char *>;
    auto GetEnviromentFromScript() -> std::vector<const char *>;
    void WaitOnStatus();
    void SetupUidGid();
    auto TrySpawn(Timeout timeout) -> ScriptStatus;

private:
    const std::string &service_name_;
    const Script &script_;
    const Environment &environment_;
    OnSuccessCallback on_success_;
};

} // namespace tt
