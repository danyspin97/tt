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

#include "tt/request/reply/system_info.hpp"

#include "tl/expected.hpp" // for expected

#include "tt/request/reply/service_info.hpp" // for ServiceInfo

tt::request::reply::SystemInfo::SystemInfo(
    const std::vector<std::reference_wrapper<const LiveService>>
        &live_services) {
    encoded_services_.reserve(live_services.size());
    for (const auto &live_service : live_services) {
        ServiceInfo info{live_service};
        encoded_services_.emplace_back(info.data());
    }
}

tt::request::reply::SystemInfo::SystemInfo(
    std::vector<std::string> &&encoded_services) {
    encoded_services_ = std::move(encoded_services);
}

auto tt::request::reply::SystemInfo::Decode()
    -> tl::expected<std::vector<LiveService>, std::string> {
    std::vector<LiveService> live_services;

    for (auto &encoded_service : encoded_services_) {
        auto ret = ServiceInfo{encoded_service}.Decode();
        if (!ret.has_value()) {
            return tl::make_unexpected(std::move(ret.error()));
        }
        live_services.emplace_back(std::move(ret.value()));
    }

    return live_services;
}
