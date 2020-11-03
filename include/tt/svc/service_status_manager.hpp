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

#include <map>          // for map
#include <memory>       // for shared_ptr
#include <shared_mutex> // for shared_mutex
#include <string>       // for string
#include <vector>       // for vector

#include "tt/svc/service_status.hpp" // for ServiceStatus
#include "tt/svc/wait_on_start.hpp"  // for WaitOnStart

namespace tt {

class ServiceStatusImpl;

class ServiceStatusManager {
public:
    explicit ServiceStatusManager(const std::vector<std::string> &services);

    void ChangeStatusOfService(const std::string &service,
                               ServiceStatus new_status);
    [[nodiscard]] auto GetUpServices() const -> std::vector<std::string>;
    [[nodiscard]] auto WaitOnServiceStart(const std::string &service) -> bool;
    void WaitOnServiceDown(const std::string &service);

private:
    static inline std::shared_mutex mutex_;
    std::map<std::string,
             std::pair<ServiceStatus, std::unique_ptr<WaitOnStart>>>
        services_;
};

} // namespace tt
