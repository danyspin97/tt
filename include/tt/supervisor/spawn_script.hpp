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

#include "tt/data/service.hpp"
#include "tt/supervisor/supervisor.hpp"
#include "tt/supervisor/types.hpp"

namespace tt {

class SpawnScript {
    enum class ScriptStatus { Failure = 0, Success = 1 };

public:
    explicit SpawnScript(const Script &script, const Environment &environment,
                         OnSuccessCallback &&on_success);
    virtual ~SpawnScript() = default;

    [[nodiscard]] auto GetStatus() const noexcept -> ScriptStatus;

    auto GetCurrentScriptStatusFromProcessFd() -> ScriptStatus;
    auto Spawn() -> ScriptStatus;
    void SetupUidGid();
    auto SetupProcessFdAndGetSupervisorFd() -> PipeFd;
    auto TrySpawn() -> ScriptStatus;

private:
    const Script &script_;
    const Environment &environment_;
    PipeFd process_fd_;
    OnSuccessCallback on_success_;
};

} // namespace tt
