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

#include <optional> // for optional
#include <utility>  // for move

#include "tt/data/main_script.hpp"               // for MainScript
#include "tt/data/oneshot.hpp"                   // for Oneshot
#include "tt/data/script.hpp"                    // for Script
#include "tt/log/oneshot_logger.hpp"             // for OneshotLogger
#include "tt/log/script_logger.hpp"              // for ScriptLogger
#include "tt/supervision/oneshot_supervisor.hpp" // for OneshotSupervisor
#include "tt/supervision/script_supervisor.hpp"  // for ScriptSupervisor
#include "tt/supervision/types.hpp"              // for ScriptStatus

tt::OneshotSupervisor::OneshotSupervisor(Oneshot oneshot,
                                         OneshotLogger &&logger)
    : oneshot_(std::move(oneshot)), logger_(std::move(logger)) {}

auto tt::OneshotSupervisor::Start() const -> bool {
    const auto &name = oneshot_.name();
    ScriptSupervisor spawn_start{name, oneshot_.start(), oneshot_.environment(),
                                 logger_.GetScriptLogger()};
    if (spawn_start.ExecuteScript() == ScriptStatus::Success) {
        return true;
    }

    if (oneshot_.stop()) {
        ScriptSupervisor spawn_stop{name, oneshot_.stop().value(),
                                    oneshot_.environment(),
                                    logger_.GetScriptLogger()};
        // We don't care if the stop script failed
        spawn_stop.ExecuteScript();
    }
    return false;
}

void tt::OneshotSupervisor::Stop() const {
    if (!oneshot_.stop()) {
        return;
    }
    const auto &name = oneshot_.name();
    ScriptSupervisor supervisor{name, oneshot_.stop().value(),
                                oneshot_.environment(),
                                logger_.GetScriptLogger()};
    supervisor.ExecuteScript();
}
