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

#include "tt/request/reply/reply.hpp" // for Reply
#include "tt/svc/live_service.hpp"    // for LiveService

namespace tt::request::reply {

class ServiceInfo : public Reply {
public:
    explicit ServiceInfo(const LiveService &live_service);
    explicit ServiceInfo(std::string encoded_info);

    [[nodiscard]] auto data() const -> std::string {
        return encoded_service_info_;
    }
    [[nodiscard]] auto Decode() -> tl::expected<LiveService, std::string>;

private:
    std::string encoded_service_info_;
};

} // namespace tt::request::reply
