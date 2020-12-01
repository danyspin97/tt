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

#include "tt/request/notify_service_status.hpp"

#include <utility> // for move

#include "tt/svc/live_service_graph.hpp" // for LiveServiceGraph

tt::request::NotifyServiceStatus::NotifyServiceStatus(std::string service,
                                                      ServiceStatus status)
    : service_(std::move(service)), status_(status) {}

auto tt::request::NotifyServiceStatus::service() const -> std::string {
    return service_;
}

auto tt::request::NotifyServiceStatus::status() const -> ServiceStatus {
    return status_;
}
