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

#include <utility>

#include "tt/supervision/oneshot_supervisor.hpp"

#include "spdlog/spdlog.h"

#include "tt/supervision/script_supervisor.hpp"
#include "tt/supervision/service_status_manager.hpp"

tt::OneshotSupervisor::OneshotSupervisor(Oneshot oneshot,
                                         std::shared_ptr<Dirs> dirs)
    : oneshot_(std::move(oneshot)), logger_(oneshot.name(), std::move(dirs)) {}

void tt::OneshotSupervisor::Run() const {
    auto manager = ServiceStatusManager::GetInstance();
    const auto &name = oneshot_.name();
    logger_.Start();
    ScriptSupervisor spawn_start{name, oneshot_.start(), oneshot_.environment(),
                                 logger_.GetScriptLogger()};
    if (spawn_start.ExecuteScript() == ScriptStatus::Success) {
        // Notify the service started up successfully
        manager.ServiceStartUpdate(name, true);
        logger_.Success();
        return;
    }

    manager.ServiceStartUpdate(name, false);
    logger_.Fail();
    if (oneshot_.stop()) {
        ScriptSupervisor spawn_stop{name, oneshot_.stop().value(),
                                    oneshot_.environment(),
                                    logger_.GetScriptLogger()};
        // We don't care if the stop script failed
        spawn_stop.ExecuteScript();
    }
}
