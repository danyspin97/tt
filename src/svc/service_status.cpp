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

#include "tt/svc/service_status.hpp"

#include <cassert> // for assert
#include <map>     // for map

#include "tt/exception.hpp" // for Exception

auto tt::GetServiceStatusName(tt::ServiceStatus status) -> std::string {
    switch (status) {
    case ServiceStatus::Reset:
        return "reset";
    case ServiceStatus::Starting:
        return "starting";
    case ServiceStatus::Stopping:
        return "stopping";
    case ServiceStatus::Up:
        return "up";
    case ServiceStatus::Down:
        return "down";
    default:
        assert(false);
    }
}

auto tt::ParseServiceStatusFromString(const std::string &status)
    -> ServiceStatus {
    static const std::map<std::string, ServiceStatus> signal_strings{
        {"reset", ServiceStatus::Reset},
        {"starting", ServiceStatus::Starting},
        {"stopping", ServiceStatus::Stopping},
        {"up", ServiceStatus::Up},
        {"down", ServiceStatus::Down}};
    if (auto itr = signal_strings.find(status); itr != signal_strings.end()) {
        return itr->second;
    }

    const auto error_msg = "Signal \"" + status + "\" is not valid";
    throw tt::Exception(error_msg);
}
