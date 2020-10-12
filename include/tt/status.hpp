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

#include <filesystem>

#include "tt/dependency_graph/dependency_graph.hpp"

namespace tt {

class Dirs;
class DependencyGraph;

class Status {
public:
    Status(const Status &) = delete;
    auto operator=(const Status &) -> Status & = delete;

    [[nodiscard]] static auto GetInstance() -> Status &;

    [[nodiscard]] auto dirs() const -> const Dirs &;
    [[nodiscard]] auto graph() -> const DependencyGraph &;

    [[nodiscard]] auto IsSystem() const -> bool;

private:
    Status();

    [[nodiscard]] static auto
    ReadGraphFromFile(std::filesystem::path &&graph_path) 
        -> tt::DependencyGraph;

    bool is_system_;
    Dirs &dirs_;
    DependencyGraph graph_;
};

} // namespace tt
