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

#include "tt/cli/command/enable_command.hpp"

#include <unistd.h>

#include "spdlog/spdlog.h"

#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/exception.hpp"
#include "tt/parser/service/system_services_parser.hpp"
#include "tt/parser/service/user_services_parser.hpp"
#include "tt/status.hpp"
#include "tt/path/user_dirs.hpp"
#include "tt/utils/read_buffer_from_file.hpp"
#include "tt/utils/serialize.hpp"

tt::cli::EnableCommand::EnableCommand(
    args::Subparser &parser, std::shared_ptr<GlobalOptions> common_options)
    : Command(parser, std::move(common_options)),
      service_list_(parser, "services", "services to parse"),
      is_root_(!(geteuid() > 0)),
      parser_(is_root_
                  ? std::make_unique<ServicesParser>(SystemServicesParser{})
                  : std::make_unique<ServicesParser>(UserServicesParser{})) {}

auto tt::cli::EnableCommand::Execute() -> int { return EnableServices(); }

auto tt::cli::EnableCommand::EnableServices() -> int {
    const auto &services_list = args::get(service_list_);
    parser_->ParseServices(services_list);
    const std::vector<tt::Service> services = parser_->services();

    const auto &status = Status::GetInstance();
    auto graph = status.graph();

    // TODO: Check if the service is already enabled
    // and if it is different
    graph.AddServices(services_list, services);

    auto graph_path = dirs()->statedir() / "graph";
    utils::Serialize(graph, graph_path);
    return 0;
}
