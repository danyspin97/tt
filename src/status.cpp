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

#include "bitsery/adapter/buffer.h"
#include "bitsery/bitsery.h"

#include "tt/exception.hpp"
#include "tt/user_dirs.hpp"
#include "tt/utils/read_buffer_from_file.hpp"

auto tt::Status::GetInstance() -> tt::Status & {
    static Status status;
    return status;
}

tt::Status::Status()
    : is_system_(!(geteuid() > 0)),
      dirs_(is_system_ ? Dirs::GetInstance() : UserDirs::GetInstance()),
      graph_(ReadGraphFromFile(dirs_.statedir() / "graph")) {}

auto tt::Status::dirs() const -> const Dirs & { return dirs_; }

auto tt::Status::graph() -> const DependencyGraph & { return graph_; }

auto tt::Status::IsSystem() const -> bool { return is_system_; }

auto tt::Status::ReadGraphFromFile(std::filesystem::path &&graph_path) const
    -> tt::DependencyGraph {
    tt::DependencyGraph graph;
    auto buffer = utils::ReadBufferFromFile(std::move(graph_path));
    auto state = bitsery::quickDeserialization<
        bitsery::InputBufferAdapter<std::vector<uint8_t>>>(
        {buffer.begin(), buffer.size()}, graph);
    if (state.first == bitsery::ReaderError::NoError && state.second) {
        return graph;
    }
    throw tt::Exception("Error reading from serialized data");
}
