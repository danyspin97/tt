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

#include <filesystem> // for operator!=, path
#include <iosfwd>     // for streamsize
#include <utility>    // for move

#include "args.hxx" // for Flag, PositionalList

// Include operator<<(ostream, Service) before ostream.h
#include "tt/data/service.hpp" // for operator<<

#include "fmt/ostream.h" // for formatbuf<>::int_type

#include "tt/log/cli_logger.hpp"                // for CliLogger
#include "tt/parser/service/service_parser.hpp" // for ServiceParser
#include "tt/path/dirs.hpp"                     // for Dirs

namespace tt::cli {
class GlobalOptions;
} // namespace tt::cli

tt::cli::ParseCommand::ParseCommand(args::Subparser &parser)
    : Command(parser), service_list_(parser, "services", "services to parse"),
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
            logger()->LogError("File {} does not exist", service);
            continue;
        }

        auto parser = ServiceParser(service);
        logger()->LogInfo("{}", parser.service());
    }
}

void tt::cli::ParseCommand::ParseUserSystemServices() {
    for (auto &&service : service_list_) {
        bool found = false;
        for (const auto &name : GetPossibleNameForService(service)) {
            if ((found = ParseForFileInDefaultDirs(name))) {
                break;
            }
        }

        if (!found) {
            logger()->LogWarn("Service '{}' could not be found", service);
        }
    }
}

auto tt::cli::ParseCommand::ParseForFileInDefaultDirs(
    const std::string &name) const -> bool {
    auto servicedirs = dirs()->servicedirs();
    for (const auto &servicedir : servicedirs) {
        auto filename = servicedir / name;
        if (std::filesystem::exists(filename)) {
            auto parser = ServiceParser(filename);
            logger()->LogInfo("{}", parser.service());
            return true;
        }
    }
    return false;
}

auto tt::cli::ParseCommand::GetPossibleNameForService(
    const std::string &service) -> std::vector<std::string> {
    auto possible_names = std::vector<std::string>{service};
    std::filesystem::path service_file{service};
    if (auto ext = service_file.extension();
        ext == "" || (ext != "system" && ext != "user")) {
        possible_names.push_back(service + ".system");
        possible_names.push_back(service + ".user");
    }
    return possible_names;
}
