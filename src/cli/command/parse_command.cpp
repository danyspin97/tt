/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#include "tt/cli/command/parse_command.hpp"

#include <filesystem>
#include <sys/stat.h>
#include <utility>

#include "args.hxx"

// Include operator<<(ostream, Service) before ostream.h
#include "tt/data/service.hpp"

#include "fmt/ostream.h" // Needed to use operator<< on custom class
#include "spdlog/spdlog.h"

#include "tt/dirs.hpp"
#include "tt/parser/service/service_parser.hpp"

#include "tt/cli/global_options.hpp"

tt::cli::ParseCommand::ParseCommand(
    args::Subparser &parser, std::shared_ptr<GlobalOptions> common_options)
    : Command(parser, std::move(common_options)),
      service_list_(parser, "services", "services to parse"),
      is_file_(parser, "file", "", {'f', "file"}) {}

auto tt::cli::ParseCommand::Dispatch(
    args::Subparser &parser, std::shared_ptr<GlobalOptions> common_options)
    -> int {
    ParseCommand command = ParseCommand(parser, std::move(common_options));
    return command.InitAndExecute();
}

auto tt::cli::ParseCommand::Execute() -> int {
    if (is_file_) {
        ParseFiles();
        return 0;
    }
    ParseUserSystemServices();
    return 0;
}

void tt::cli::ParseCommand::ParseFiles() const {
    for (auto &&service : service_list_) {
        auto parser = ServiceParser(service);
        spdlog::info(parser.service());
    }
}

void tt::cli::ParseCommand::ParseUserSystemServices() {
    // TODO: Check for UID != 0 and add xdg.userservice
    for (auto &&service : service_list_) {
        bool found = false;
        for (const auto &name : GetPossibleNameForService(service)) {
            found = CheckForFileInDefaultDirs(name);
            if (found) {
                break;
            }
        }

        if (!found) {
            spdlog::info("Service '{}' could not be found", service);
        }
    }
}

auto tt::cli::ParseCommand::CheckForFileInDefaultDirs(
    const std::string &name) -> bool {
    tt::Dirs &dirs = tt::Dirs::GetInstance();
    auto default_dirs = std::vector<std::string>{dirs.servicedir(),
                                                 dirs.confdir() + "/service"};
    for (auto i = default_dirs.rbegin(); i != default_dirs.rend(); ++i) {
        auto filename = *i + "/" + name;
        struct stat buffer {};
        if (stat(filename.c_str(), &buffer) == 0) {
            auto parser = ServiceParser(name);
            spdlog::info(parser.service());
            return true;
        }
    }
    return false;
}

auto tt::cli::ParseCommand::GetPossibleNameForService(
    const std::string &service) -> std::vector<std::string> {
    auto possible_names = std::vector<std::string>{service};
    std::filesystem::path service_path{service};
    if (auto ext = service_path.extension();
        ext != "" && ext != (".system") && ext != ".user") {
        possible_names.push_back(service + ".system");
        possible_names.push_back(service + ".user");
    }
    return possible_names;
}
