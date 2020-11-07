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

#include <string> // for string

#include "tt/request/request.hpp"    // for Request
#include "tt/svc/service_status.hpp" // for ServiceStatus

namespace tt::request {

class NotifyServiceStatus : public Request {
public:
    NotifyServiceStatus(std::string service, ServiceStatus status);
    NotifyServiceStatus() = delete;

    [[nodiscard]] auto service() const -> std::string;
    [[nodiscard]] auto status() const -> ServiceStatus;

    static inline const std::string request_name = "notify_service_status";

private:
    std::string service_;
    ServiceStatus status_;
};

} // namespace tt::request
