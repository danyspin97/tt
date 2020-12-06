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

#include "tt/request/request.hpp" // for Request
#include "tt/request/visited.hpp" // for Visited

namespace tt::request {

class ServiceStatusRequest : public Visited<ServiceStatusRequest> {
public:
    explicit ServiceStatusRequest(std::string service)
        : service_(std::move(service)) {}
    ServiceStatusRequest() = delete;

    [[nodiscard]] auto service() const -> std::string { return service_; }
    [[nodiscard]] auto service() -> std::string && {
        return std::move(service_);
    }

    static inline const std::string request_name = "show_service_status";

private:
    std::string service_;
};

} // namespace tt::request
