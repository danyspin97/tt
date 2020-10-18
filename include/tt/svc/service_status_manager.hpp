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

#include <map>
#include <shared_mutex>
#include <vector>

namespace tt {

class ServiceStatus;

class ServiceStatusManager {
public:
    static auto GetInstance() -> ServiceStatusManager &;

    void Initialize(std::vector<std::string> &services);
    void ServiceStartUpdate(const std::string &service, bool succeeded) const;
    void ServiceDownUpdate(const std::string &service, bool succeeded) const;
    auto WaitOnServiceStart(const std::string &service) const -> bool;
    auto WaitOnServiceDown(const std::string &service) const -> bool;

private:
    using ServiceStatusPtr = std::shared_ptr<ServiceStatus>;

    static auto
    GetStatus(const std::map<std::string, ServiceStatusPtr> &services_status,
              const std::string &service) -> ServiceStatusPtr;

    static std::shared_mutex mutex_;
    std::map<std::string, ServiceStatusPtr> up_status_;
    std::map<std::string, ServiceStatusPtr> down_status_;
};

} // namespace tt
