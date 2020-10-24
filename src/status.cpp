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

#include "spdlog/spdlog.h"

#include "tt/define.h"
#include "tt/exception.hpp"
#include "tt/path/user_dirs.hpp"
#include "tt/utils/deserialize.hpp"
#include "tt/utils/read_buffer_from_file.hpp"

auto tt::Status::GetInstance() -> tt::Status & {
    static Status status;
    return status;
}

tt::Status::Status()
    : dirs_(is_system_ ? Dirs::GetInstance() : UserDirs::GetInstance()),
      graph_(ReadGraphFromFileOrNew(dirs_.statedir() / "graph")) {}

auto tt::Status::dirs() const -> const Dirs & { return dirs_; }

auto tt::Status::graph() const -> const DependencyGraph & { return graph_; }

auto tt::Status::IsSystem() const -> bool { return is_system_; }

auto tt::Status::ReadGraphFromFileOrNew(const std::filesystem::path &graph_path)
    -> tt::DependencyGraph {
    if (!std::filesystem::exists(graph_path)) {
        return DependencyGraph{};
    }
    return utils::Deserialize<tt::DependencyGraph>(graph_path);
}
