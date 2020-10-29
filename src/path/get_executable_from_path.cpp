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

#include "tt/path/get_executable_from_path.hpp"

#include <cassert> // for assert

#include "fmt/format.h" // for format

#include "tt/exception.hpp"        // for Exception
#include "tt/utils/split_path.hpp" // for SplitPath

auto tt::GetExecutableFromPath(const std::string &executable)
    -> std::filesystem::path {
    assert(std::filesystem::path{executable}.is_relative());
    // TODO: Use tt.env or Status::env()
    const char *env_path = getenv("PATH");
    std::string path_to_search;
    if (env_path != nullptr) {
        path_to_search = std::string{env_path};
    } else {
        path_to_search = "/bin:/sbin:/usr/bin:/usr/sbin";
    }
    auto paths = utils::SplitPath(path_to_search);
    for (const auto &p : paths) {
        std::filesystem::path new_path{p / executable};
        if (std::filesystem::exists(new_path)) {
            // TODO: Check file is executable
            return new_path;
        }
    }

    throw Exception(
        fmt::format("Could not find {} in PATH='{}'", executable, env_path));
}
