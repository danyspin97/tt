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

#include "tt/supervision/service_status_manager.hpp"

#include <cassert>

#include "fmt/format.h"

#include "tt/exception.hpp"
#include "tt/supervision/service_status.hpp"

auto tt::ServiceStatusManager::GetInstance() -> ServiceStatusManager & {
    static ServiceStatusManager instance;
    return instance;
}

void tt::ServiceStatusManager::Initialize(std::vector<std::string> &services) {
    for (auto &service : services) {
        // TODO: Use a vector to allocate the ServiceStatus objects
        up_status_.emplace(service, std::make_shared<ServiceStatus>());
        down_status_.emplace(service, std::make_shared<ServiceStatus>());
    }
}

auto tt::ServiceStatusManager::GetStatus(
    const std::map<std::string, ServiceStatusPtr> &services_status,
    const std::string &service) -> ServiceStatusPtr {
    auto status = services_status.find(service);
    if (status == services_status.end()) {
        auto err_msg = fmt::format("Unexpected service {}", service);
        throw Exception(err_msg);
    }

    assert(status->second);
    return status->second;
}

void tt::ServiceStatusManager::ServiceStartUpdate(const std::string &service,
                                                  bool succeeded) const {
    // Use a shared lock to prevent other methods on writing on the map
    std::shared_lock lock{mutex_};
    GetStatus(up_status_, service)->Update(succeeded);
}

void tt::ServiceStatusManager::ServiceDownUpdate(const std::string &service,
                                                 bool succeeded) const {
    std::shared_lock lock{mutex_};
    GetStatus(down_status_, service)->Update(succeeded);
}

auto tt::ServiceStatusManager::WaitOnServiceStart(
    const std::string &service) const -> bool {
    // Use a shared lock to prevent other methods on writing on the map
    std::shared_lock lock{mutex_};
    return GetStatus(up_status_, service)->Wait();
}

auto tt::ServiceStatusManager::WaitOnServiceDown(
    const std::string &service) const -> bool {
    // Use a shared lock to prevent other methods on writing on the map
    std::shared_lock lock{mutex_};
    return GetStatus(down_status_, service)->Wait();
}
