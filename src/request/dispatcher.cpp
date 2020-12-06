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

#include "tt/request/dispatcher.hpp"

#include "tt/request/notify_service_status.hpp"  // for NotifyServiceStatus
#include "tt/request/service_status_request.hpp" // for ServiceStatus

tt::request::Dispatcher::Dispatcher(LiveServiceGraph &live_graph)
    : live_graph_(live_graph) {}

auto tt::request::Dispatcher::operator()(
    std::shared_ptr<NotifyServiceStatus> notify) -> std::optional<std::string> {
    live_graph_.ChangeStatusOfService(notify->service(), notify->status());
    return {};
}

auto tt::request::Dispatcher::operator()(
    std::shared_ptr<ServiceStatusRequest> status)
    -> std::optional<std::string> {
    return {};
}
