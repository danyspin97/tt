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

#include <memory>   // for unique_ptr
#include <optional> // for optional

#include "nlohmann/json_fwd.hpp" // for json

#include "tl/expected.hpp" // for expected

namespace tt::request {

class NotifyServiceStatus;
class Reply;
class ServiceInfo;
class SystemInfo;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual auto operator()(std::shared_ptr<NotifyServiceStatus> notify)
        -> tl::expected<nlohmann::json, std::string> = 0;

    virtual auto operator()(std::shared_ptr<ServiceInfo> status)
        -> tl::expected<nlohmann::json, std::string> = 0;

    virtual auto operator()(std::shared_ptr<SystemInfo> status)
        -> tl::expected<nlohmann::json, std::string> = 0;
};

} // namespace tt::request
