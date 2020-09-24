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

#include "tt/svc/service_status_manager.hpp"

#include <cassert>

#include "fmt/format.h"

#include "tt/exception.hpp"
#include "tt/svc/service_status.hpp"

tt::ServiceStatusManager::~ServiceStatusManager() {
    for (auto &pair : up_status_) {
        delete pair.second;
    }
}

auto tt::ServiceStatusManager::GetInstance() -> ServiceStatusManager & {
    static ServiceStatusManager instance;
    return instance;
}

void tt::ServiceStatusManager::Initialize(std::vector<std::string> &services) {
    for (auto &service : services) {
        // TODO: Use a vector to allocate the ServiceStatus objects
        up_status_.emplace(service, new ServiceStatus());
    }
}

void tt::ServiceStatusManager::ServiceHasStarted(const std::string &service) {
    auto status = up_status_.find(service);
    if (status == up_status_.end()) {
        auto err_msg = fmt::format("Unexpected service {}", service);
        throw Exception(err_msg);
    }
    assert(status->second);
    status->second->ServiceUp();
}

void tt::ServiceStatusManager::WaitOnService(const std::string &service) {
    auto status = up_status_.find(service);
    if (status == up_status_.end()) {
        auto err_msg = fmt::format("Unexpected service {}", service);
        throw Exception(err_msg);
    }
    assert(status->second);
    status->second->Wait();
}
