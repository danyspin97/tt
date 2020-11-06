/*
 * Copyright (c) 2020 Rasmus Thomsen <oss@cogitri.dev>
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

#include <iostream> // for operator<<, cout
#include <memory>   // for allocator, mak...
#include <string>   // for string

#include "args.hxx"       // for ArgumentParser
#include "spdlog/async.h" // for init_thread_pool

#include "tt/cli/command/command_dispatcher.hpp"      // for CommandDispatcher
#include "tt/cli/command/disable_command.hpp"         // for DisableCommand
#include "tt/cli/command/edit_config_command.hpp"     // for EditConfigCommand
#include "tt/cli/command/enable_command.hpp"          // for EnableCommand
#include "tt/cli/command/parse_command.hpp"           // for ParseCommand
#include "tt/cli/command/serialize_command.hpp"       // for SerializeCommand
#include "tt/cli/command/service_control_command.hpp" // for ServiceControl...
#include "tt/cli/command/show_command.hpp"            // for ShowCommand
#include "tt/cli/command/start_command.hpp"           // for StartCommand
#include "tt/cli/command/status_command.hpp"          // for StatusCommand
#include "tt/cli/command/stop_command.hpp"            // for StopCommand
#include "tt/cli/command/supervise_command.hpp"       // for SuperviseCommand
#include "tt/cli/global_options.hpp"                  // for GlobalOptions

template <typename T>
void AddCommandToGroup(args::Group &group,
                       std::vector<std::unique_ptr<args::Command>> &commands,
                       tt::cli::CommandDispatcher &dispatcher) {
    commands.push_back(std::make_unique<args::Command>(
        group, T::name, T::description, dispatcher.Dispatch<T>()));
}

auto main(int argc, char *argv[]) -> int {
    spdlog::init_thread_pool(512, 1);

    std::vector<std::unique_ptr<args::Command>> commands;
    args::ArgumentParser parser("tt init/rc manager.");
    auto common_options = std::make_shared<tt::cli::GlobalOptions>();
    tt::cli::CommandDispatcher dispatcher{common_options};
    args::GlobalOptions global_options((args::Group &)parser,
                                       common_options->arguments());

    args::Group manage{(args::Group &)parser, "Manage services"};
    args::Group system_group((args::Group &)parser, "System commands");
    args::Group query((args::Group &)parser, "Query the status of the system");
    args::Group testing((args::Group &)parser, "Test services files");

    AddCommandToGroup<tt::cli::EnableCommand>(manage, commands, dispatcher);
    AddCommandToGroup<tt::cli::DisableCommand>(manage, commands, dispatcher);
    AddCommandToGroup<tt::cli::StartCommand>(manage, commands, dispatcher);
    AddCommandToGroup<tt::cli::StopCommand>(manage, commands, dispatcher);
    AddCommandToGroup<tt::cli::EditConfigCommand>(manage, commands, dispatcher);

    AddCommandToGroup<tt::cli::StatusCommand>(manage, commands, dispatcher);
    AddCommandToGroup<tt::cli::ShowCommand>(manage, commands, dispatcher);

    AddCommandToGroup<tt::cli::ServiceControlCommand>(system_group, commands,
                                                      dispatcher);
    AddCommandToGroup<tt::cli::SuperviseCommand>(system_group, commands,
                                                 dispatcher);

    AddCommandToGroup<tt::cli::ParseCommand>(testing, commands, dispatcher);
    AddCommandToGroup<tt::cli::SerializeCommand>(testing, commands, dispatcher);

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help & /*e*/) {
        parser.Help(std::cout);
    } catch (const args::Error &e) {
        std::cerr << e.what() << std::endl;
        parser.Help(std::cout);
        return 1;
    }

    return dispatcher.exit_code();
}
