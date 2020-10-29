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

#include "tt/path/split_path.hpp"

#include <algorithm> // for find, max
#include <cstddef>   // for size_t
#include <iterator>  // for begin, end

auto tt::SplitPath(const std::string &path)
    -> std::vector<std::filesystem::path> {
    std::vector<std::filesystem::path> paths{};
    size_t start = 0;
    size_t pos = 0;
    while ((pos = path.find_first_of(':', start)) != std::string::npos) {
        std::filesystem::path current_path{path.substr(start, pos - start)};
        if (current_path.is_absolute() &&
            std::find(std::begin(paths), std::end(paths), current_path) ==
                paths.end()) {
            paths.emplace_back(current_path);
        }
        start = pos + 1;
    }
    std::filesystem::path current_path{path.substr(start, pos - start)};
    if (current_path.is_absolute() &&
        std::find(std::begin(paths), std::end(paths), current_path) ==
            paths.end()) {
        paths.emplace_back(current_path);
    }

    return paths;
}
