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

#include "tt/supervision/supervise_service.hpp"

#include <cassert> // for assert
#include <utility> // for move

#include "tt/log/longrun_logger.hpp"          // for LongrunLogger
#include "tt/log/oneshot_logger.hpp"          // for OneshotLogger
#include "tt/log/service_logger_registry.hpp" // for ServiceLoggerRegistry
#include "tt/supervision/longrun_supervisor_launcher.hpp" // for LongrunSup...
#include "tt/supervision/oneshot_supervisor.hpp"          // for OneshotSup...

tt::SuperviseService::SuperviseService(
    std::shared_ptr<Dirs> dirs,
    std::shared_ptr<ServiceLoggerRegistry> logger_registry)
    : dirs_(std::move(dirs)), logger_registry_(std::move(logger_registry)) {}

// Bundles are only used at compile time
void tt::SuperviseService::operator()(const Bundle & /*bundle*/) const {
    assert(false);
}

void tt::SuperviseService::operator()(const Longrun &longrun) const {
    LongrunSupervisorLauncher supervise{longrun, dirs_};
    supervise.Launch();
}

void tt::SuperviseService::operator()(const Oneshot &oneshot) const {
    OneshotSupervisor spawn{oneshot,
                            logger_registry_->GetOneshotLogger(oneshot.name())};
    spawn.Run();
}

void tt::SuperviseService::operator()(std::monostate /*unused*/) const {
    assert(false);
}
