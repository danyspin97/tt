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

#include "tt/dependency_graph/get_graph_filename.hpp" // for GetGraphFilename
#include "tt/request/notify_service_status.hpp"       // for NotifyServiceStatus
#include "tt/request/reply/pack_reply.hpp"            // for PackReply
#include "tt/request/reply/service_info.hpp"          // for ServiceInfo
#include "tt/request/reply/system_info.hpp"           // for SystemInfo
#include "tt/request/service_info.hpp"                // for ServiceInfo
#include "tt/request/start_services.hpp"              // for StartServices
#include "tt/request/stop_services.hpp"               // for StopServices

using nlohmann::json;

tt::request::Dispatcher::Dispatcher(LiveServiceGraph &live_graph,
                                    std::shared_ptr<Dirs> dirs)
    : live_graph_(live_graph), dirs_(std::move(dirs)) {}

auto tt::request::Dispatcher::operator()(
    std::shared_ptr<NotifyServiceStatus> notify)
    -> tl::expected<json, std::string> {
    live_graph_.ChangeStatusOfService(notify->service(), notify->status());
    return {};
}

auto tt::request::Dispatcher::operator()(
    std::shared_ptr<ReloadGraph> /*reload_graph*/)
    -> tl::expected<json, std::string> {
    auto graph_filename = GetGraphFilename(dirs_);
    if (!std::filesystem::exists(graph_filename) ||
        !std::filesystem::is_regular_file(graph_filename)) {
        return tl::make_unexpected("Could not find the serialized graph.");
    }

    live_graph_.Update(utils::Deserialize<DependencyGraph>(graph_filename));
    return {};
}

auto tt::request::Dispatcher::operator()(std::shared_ptr<ServiceInfo> status)
    -> tl::expected<json, std::string> {
    if (!live_graph_.HasService(status->service())) {
        return tl::make_unexpected(
            fmt::format("Service {} not found", status->service()));
    }

    return PackReply(reply::ServiceInfo{
        live_graph_.GetLiveServiceFromName(status->service())});
}

auto tt::request::Dispatcher::operator()(
    std::shared_ptr<StartServices> start_request)
    -> tl::expected<json, std::string> {
    std::string error_msg;

    auto services = std::move(start_request->services());
    for (auto &service_name : services) {
        if (!live_graph_.HasService(service_name)) {
            return tl::make_unexpected(
                fmt::format("Service {} not found", service_name));
        }
    }

    std::for_each(services.begin(), services.end(),
                  [this, &error_msg](auto &service_name) {
                      auto ret = live_graph_.StartService(service_name);
                      if (!ret.has_value()) {
                          fmt::format_to(std::back_inserter(error_msg), "{}",
                                         ret.error());
                          return;
                      }
                      if (ret.value() != ScriptStatus::Success) {
                          fmt::format_to(std::back_inserter(error_msg),
                                         "Service {} failed to start",
                                         service_name);
                      }
                  });
    return error_msg.empty() ? tl::expected<json, std::string>{}
                             : tl::make_unexpected(error_msg);
}

auto tt::request::Dispatcher::operator()(
    std::shared_ptr<StopServices> stop_request)
    -> tl::expected<json, std::string> {
    std::string error_msg;

    auto services = std::move(stop_request->services());
    for (auto &service_name : services) {
        if (!live_graph_.HasService(service_name)) {
            return tl::make_unexpected(
                fmt::format("Service {} not found", service_name));
        }
    }

    std::for_each(services.begin(), services.end(),
                  [this, &error_msg](auto &service_name) {
                      auto ret = live_graph_.StopService(service_name);
                      if (!ret.has_value()) {
                          fmt::format_to(std::back_inserter(error_msg), "{}",
                                         ret.error());
                      }
                  });
    return error_msg.empty() ? tl::expected<json, std::string>{}
                             : tl::make_unexpected(error_msg);
}

auto tt::request::Dispatcher::operator()(
    std::shared_ptr<SystemInfo> /*system_info*/)
    -> tl::expected<json, std::string> {
    return PackReply(reply::SystemInfo{live_graph_.live_services()});
}
