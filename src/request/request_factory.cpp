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
    -> std::pair<std::string, std::unique_ptr<Request>> try {
    json j = json::parse(buffer);

    // ComplexRequest:
    //     request_name: "complex_action"
    //     request: { request_object }
    std::string request_name = j.at("request_name");

    if (request_name == NotifyServiceStatus::request_name) {
        return std::make_pair(
            NotifyServiceStatus::request_name,
            std::make_unique<NotifyServiceStatus>(j.at("request")));
    }
    throw tt::Exception(request_name + " is not a supported request");
} catch (const nlohmann::detail::exception & /*e*/) {
    throw tt::Exception("There was an error while parsing the request");
}
