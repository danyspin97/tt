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

#include "tt/cli/command/service_control_command.hpp"

#include <filesystem> // for exists, is_reg...
#include <thread>     // for thread

#include "tt/dependency_graph/dependency_graph.hpp"   // for DependencyGraph
#include "tt/dependency_graph/get_graph_filename.hpp" // for GetGraphFilename
#include "tt/exception.hpp"                           // for Exception
#include "tt/file_lock.hpp"                           // for FileLock
#include "tt/request/request_listener.hpp"            // for ActionListener
#include "tt/supervision/signal_handler.hpp" // for AddSignalsToSet, ...
#include "tt/svc/service_manager.hpp"        // for ServiceManager
#include "tt/utils/launch_async.hpp"         // for LaunchAsync

namespace args {
class Subparser;
} // namespace args
namespace tt::cli {
class GlobalOptions;
} // namespace tt::cli

tt::cli::ServiceControlCommand::ServiceControlCommand(args::Subparser &parser)
    : Command(parser) {}

auto tt::cli::ServiceControlCommand::Execute() -> int {
    FileLock lock(dirs()->livedir() / ".svc_lock");
    if (!lock.TryLock()) {
        throw Exception("Another instance of tt-svc is running");
    }

    auto signal_set = GetEmptySignalSet();
    AddSignalsToSet(kStopSignals, &signal_set);
    MaskSignals(&signal_set);

    auto graph_filename = GetGraphFilename(dirs());
    if (!std::filesystem::exists(graph_filename) ||
        !std::filesystem::is_regular_file(graph_filename)) {
        throw Exception("Could not find the serialized graph.");
    }
    auto graph = utils::Deserialize<DependencyGraph>(graph_filename);
    auto services = graph.GetActiveServices();
    ServiceManager service_manager(std::move(graph), dirs());

    request::RequestListener request_listener(service_manager, dirs());

    // Start action listener
    std::thread(&request::RequestListener::Listen, request_listener).detach();

    tt::LaunchAsync(
        [&service_manager]() { service_manager.StartAllServices(); });

    // Wait until we receive a stop signal
    WaitOnSignalSet(&signal_set);

    service_manager.StopAllServices();

    return 0;
}
