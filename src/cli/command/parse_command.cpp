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

#include "tt/cli/command/parse_command.hpp"

#include <filesystem>
#include <utility>

#include "args.hxx"

// Include operator<<(ostream, Service) before ostream.h
#include "tt/data/service.hpp"

#include "fmt/ostream.h" // Needed to use operator<< on custom class
#include "spdlog/spdlog.h"

#include "tt/parser/service/service_parser.hpp"
#include "tt/path/dirs.hpp"

#include "tt/cli/global_options.hpp"

tt::cli::ParseCommand::ParseCommand(
    args::Subparser &parser, std::shared_ptr<GlobalOptions> common_options)
    : Command(parser, std::move(common_options)),
      service_list_(parser, "services", "services to parse"),
      is_file_(parser, "file", "", {'f', "file"}) {}

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
        if (!std::filesystem::exists(service)) {
            spdlog::error("File {} does not exist", service);
            continue;
        }

        auto parser = ServiceParser(service);
        spdlog::info(parser.service());
    }
}

void tt::cli::ParseCommand::ParseUserSystemServices() {
    // TODO: Check for UID != 0 and add xdg.userservice
    for (auto &&service : service_list_) {
        bool found = false;
        for (const auto &name : GetPossibleNameForService(service)) {
            if (found = ParseForFileInDefaultDirs(name); found) {
                break;
            }
        }

        if (!found) {
            spdlog::info("Service '{}' could not be found", service);
        }
    }
}

auto tt::cli::ParseCommand::ParseForFileInDefaultDirs(const std::string &name)
    -> bool {
    auto servicedirs = dirs()->servicedirs();
    for (const auto &servicedir : servicedirs) {
        auto filename = servicedir / name;
        if (std::filesystem::exists(filename)) {
            auto parser = ServiceParser(filename);
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
