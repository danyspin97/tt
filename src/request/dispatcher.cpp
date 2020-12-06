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

#include "Base64.h" // for Encode

#include "bitsery/adapter/buffer.h"
#include "bitsery/bitsery.h"
#include "bitsery/traits/string.h"
#include "bitsery/traits/vector.h"

#include "nlohmann/json.hpp" // for json

#include "fmt/format.h" // for format

#include "tt/request/notify_service_status.hpp"  // for NotifyServiceStatus
#include "tt/request/service_info.hpp" // for ServiceStatus

using nlohmann::json;

tt::request::Dispatcher::Dispatcher(LiveServiceGraph &live_graph)
    : live_graph_(live_graph) {}

auto tt::request::Dispatcher::operator()(
    std::shared_ptr<NotifyServiceStatus> notify) -> std::optional<std::string> {
    live_graph_.ChangeStatusOfService(notify->service(), notify->status());
    return {};
}

auto tt::request::Dispatcher::operator()(
    std::shared_ptr<ServiceInfo> status)
    -> std::optional<std::string> {
    if (!live_graph_.HasService(status->service())) {
        json j;
        j["ok"] = false;
        j["error"] = fmt::format("Service {} not found", status->service());
        return j.dump();
    }

    auto &live_service = live_graph_.GetLiveServiceFromName(status->service());

    using Buffer = std::vector<uint8_t>;
    Buffer buffer;

    bitsery::quickSerialization<bitsery::OutputBufferAdapter<Buffer>>(
        buffer, live_service);
    std::string serialized_live_service;
    std::copy(buffer.cbegin(), buffer.cend(),
              std::back_inserter(serialized_live_service));
    auto encoded_status = macaron::Base64::Encode(serialized_live_service);
    json j;
    j["ok"] = true;
    j["data"] = std::move(encoded_status);
    return j.dump();
}
