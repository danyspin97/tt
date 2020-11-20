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

#include <cstddef>       // for size_t
#include <filesystem>    // for path
#include <future>        // for future
#include <memory>        // for shared_ptr
#include <shared_mutex>  // for shared_mutex
#include <string>        // for string, hash
#include <unordered_map> // for unordered_map
#include <vector>        // for vector

#include "tt/data/service.hpp" // for Service

namespace tt {
class Dirs;

class ServicesParser {
public:
    explicit ServicesParser(const std::shared_ptr<Dirs> &dirs);

    auto ParseServices(const std::vector<std::string> &service_names)
        -> std::vector<Service>;

private:
    auto ParseService(const std::string &service_name) -> Service;
    void ParseDependenciesOfService(const Service &service);
    static auto GetInstanceTokenIndex(const std::string &service_name)
        -> size_t;
    static auto SplitServiceNameFromInstance(std::string &service,
                                             size_t token_index) -> std::string;
    auto GetPathForServiceName(const std::string &name) -> std::string;

    std::mutex services_to_parse_mutex_;
    std::mutex futures_mutex_;
    std::string suffix_;
    std::vector<std::filesystem::path> paths_;
    std::atomic_uint16_t service_to_parse_;
    std::vector<std::string> service_names_to_parse_;
    std::vector<std::future<Service>> futures_;
    std::condition_variable future_cv_;
    std::condition_variable services_cv_;
};
} // namespace tt
