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

#include "tt/request/request_factory.hpp"

#include <string> // for string, operator==

#include <nlohmann/json.hpp> // for json, detail::exception

#include "tt/exception.hpp"                             // for Exception
#include "tt/request/adapter/notify_service_statup.hpp" // IWYU pragma: keep
#include "tt/request/notify_service_status.hpp" // for NotifyServiceStatus

using json = nlohmann::json;

auto tt::request::RequestFactory::GetRequestFromBuffer(
    const std::string &buffer)
    -> tl::expected<std::shared_ptr<Request>, std::string> {
    try {
        json j = json::parse(buffer);

        // ComplexRequest:
        //     request_name: "complex_action"
        //     request: { request_object }
        std::string request_name = j.at("request_name");

        if (request_name == NotifyServiceStatus::request_name) {
            return std::make_shared<NotifyServiceStatus>(j.at("request"));
        }
        return tl::make_unexpected(request_name +
                                   " is not a valid request name");
    } catch (const nlohmann::detail::exception &e) {
        return tl::make_unexpected(e.what());
    }
}
