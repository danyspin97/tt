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

#include "pstream.h"

#include "tt/data/service.hpp"
#include "tt/svc/spawn_supervise.hpp"
#include "tt/svc/types.hpp"

namespace spdlog {
class logger;
}

namespace tt {

class Timeout;

class SpawnScript {
public:
    explicit SpawnScript(const std::string &service_name, const Script &script,
                         const Environment &environment);
    virtual ~SpawnScript() = default;

    auto Spawn() -> ScriptStatus;
    [[nodiscard]] auto service_name() const -> const std::string &;
    void Kill();
    virtual void Signal(int signum);

protected:
    auto GetEnviromentFromScript() -> std::vector<const char *>;
    virtual auto HasExited() -> bool;
    void SetupUidGid();
    auto TrySpawn(Timeout timeout) -> ScriptStatus;

    void InitLogger();

private:
    const std::string &service_name_;
    Script script_;
    const Environment &environment_;
    std::shared_ptr<spdlog::logger> logger_;

    redi::ipstream proc_;
};

} // namespace tt
