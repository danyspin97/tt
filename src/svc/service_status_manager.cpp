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

#include "tt/svc/service_status_manager.hpp"

#include <cassert> // for assert
#include <utility> // for pair

#include "fmt/format.h" // for format

#include "tt/exception.hpp" // for Exception

tt::ServiceStatusManager::ServiceStatusManager(
    const std::vector<std::string> &services) {
    for (auto service : services) {
        services_.emplace(service,
                          std::make_pair(ServiceStatus::Reset,
                                         std::make_unique<WaitOnStart>()));
    }
}

void tt::ServiceStatusManager::ChangeStatusOfService(const std::string &service,
                                                     ServiceStatus new_status) {
    // We are going to write into the map, do not allow other threads to read
    std::unique_lock lock{mutex_};
    auto &service_data = services_.at(service);
    service_data.first = new_status;

    switch (new_status) {
    case ServiceStatus::Up:
    case ServiceStatus::Down:
        service_data.second->Signal();
    default:
        // No need to do anything here
        ;
    }
}

auto tt::ServiceStatusManager::WaitOnServiceStart(const std::string &service)
    -> bool {
    auto &service_data = services_.at(service);
    service_data.second->Wait();

    // We only need the shared_lock when reading from services_ map
    std::shared_lock lock{mutex_};
    return service_data.first == ServiceStatus::Up;
}

void tt::ServiceStatusManager::WaitOnServiceDown(const std::string &service) {
    services_.at(service).second->Wait();
}

auto tt::ServiceStatusManager::GetUpServices() const
    -> std::vector<std::string> {
    std::shared_lock lock(mutex_);
    std::vector<std::string> up_services;
    for (const auto &entry : services_) {
        if (entry.second.first == ServiceStatus::Up) {
            up_services.push_back(entry.first);
        }
    }

    return up_services;
}
