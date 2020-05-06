/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "tt/data/service.hpp"

namespace tt {

class ServicesParser {
public:
    std::vector<std::unique_ptr<Service>> services();
    void ParseServices(const std::vector<std::string> &service_names);

protected:
    ServicesParser(const std::string &suffix,
                   const std::vector<std::string> &paths);

private:
    void ParseService(const std::string &service_name);
    void ParseDependenciesOfService(const std::string &name, Service *service);
    static size_t GetInstanceTokenIndex(const std::string &service_name);
    static std::string SplitServiceNameFromInstance(std::string &service,
                                                    size_t token_index);
    std::string GetPathForServiceName(const std::string &name);

protected:
    std::string suffix_;
    std::vector<std::string> paths_;

private:
    std::unordered_map<std::string, std::shared_ptr<Service>> service_map_;
};
} // namespace tt
