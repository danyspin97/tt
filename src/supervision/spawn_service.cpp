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

#include "tt/supervision/spawn_service.hpp"

#include <unistd.h>

#include <filesystem>

#include "tt/data/service.hpp"
#include "tt/supervision/longrun_supervisor_launcher.hpp"
#include "tt/supervision/oneshot_supervisor.hpp"

// Bundles are only used at compile time
void tt::SpawnService::operator()(const Bundle & /*bundle*/) const {
    assert(false);
}

void tt::SpawnService::operator()(const Longrun &longrun) const {
    LongrunSupervisorLauncher supervise{longrun};
    supervise.Spawn();
}

void tt::SpawnService::operator()(const Oneshot &oneshot) const {
    OneshotSupervisor spawn{oneshot};
    spawn.Run();
}

void tt::SpawnService::operator()(std::monostate /*unused*/) const {
    assert(false);
}
