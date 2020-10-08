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

#include "tt/svc/spawn_service.hpp"

#include <unistd.h>

#include <filesystem>

#include "tt/serialize.hpp"
#include "tt/svc/spawn_oneshot.hpp"
#include "tt/svc/spawn_supervise.hpp"
#include "tt/user_dirs.hpp"

tt::SpawnService::SpawnService(const Service &service) {
    if constexpr (std::is_same_v<std::decay_t<decltype(service)>, Longrun>) {
        SpawnSupervise(std::get<Longrun>(service));
    } else if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                        Oneshot>) {
        SpawnOneshot(std::get<Oneshot>(service));
    } else if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                        std::monostate>) {
        assert(false);
    }
}
