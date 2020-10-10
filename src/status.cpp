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

#include "tt/status.hpp"

#include <unistd.h>

#include "tt/user_dirs.hpp"

auto tt::Status::GetInstance() -> tt::Status & {
    return SetupInstance(DependencyGraph{});
}

auto tt::Status::SetupInstance(tt::DependencyGraph &&graph) -> tt::Status & {
    static Status status(std::move(graph));
    return status;
}

tt::Status::Status(DependencyGraph &&graph)
    : graph_(std::move(graph)), is_system_(!(geteuid() > 0)),
      dirs_(is_system_ ? Dirs::GetInstance() : UserDirs::GetInstance()) {}

auto tt::Status::graph() -> const DependencyGraph & { return graph_; }
