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
#include <iostream>
#include <sys/stat.h>

#include "args.hxx"

#include "tt/dirs.h"
#include "tt/parser/service/service_parser.hpp"

#include "tt/cli/global_options.hpp"

using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::vector;

using tt::dirs;
using tt::ServiceParser;

using tt::cli::GlobalOptions;
using tt::cli::ParseCommand;

ParseCommand::ParseCommand(args::Subparser &parser,
                           shared_ptr<GlobalOptions> common_options)
    : Command(parser, common_options),
      service_list_(parser, "services", "services to parse"),
      is_file_(parser, "file", "", {'f', "file"}) {}

int ParseCommand::Dispatch(args::Subparser &parser,
                           shared_ptr<GlobalOptions> common_options) {
    ParseCommand command = ParseCommand(parser, common_options);
    return command.InitAndExecute();
}

int ParseCommand::Execute() {
    if (is_file_) {
        ParseFiles();
        return 0;
    }
    ParseUserSystemServices();
    return 0;
}

void ParseCommand::ParseFiles() {
    for (auto &&service : service_list_) {
        auto parser = ServiceParser(service);
        if (!common_options_->quiet_) {
            std::cout << *(parser.service());
        }
        continue;
    }
}

void ParseCommand::ParseUserSystemServices() {
    // TODO: Check for UID != 0 and add xdg.userservice
    for (auto &&service : service_list_) {
        bool found = false;
        for (auto &name : GetPossibleNameForService(service)) {
            found = CheckForFileInDefaultDirs(name);
            if (found) {
                break;
            }
        }

        if (!found) {
            cout << "Service '" << service << " could not be found" << endl;
        }
    }
}

bool ParseCommand::CheckForFileInDefaultDirs(const std::string &name) {
    auto default_dirs = vector<string>{dirs.service, dirs.adminService};
    for (vector<string>::reverse_iterator i = default_dirs.rbegin();
         i != default_dirs.rend(); ++i) {
        auto filename = *i + "/" + name;
        struct stat buffer;
        if (stat(filename.c_str(), &buffer) == 0) {
            auto parser = ServiceParser(name);
            if (!common_options_->quiet_) {
                cout << parser.service() << endl;
            }
            return true;
        }
    }
    return false;
}

vector<string>
ParseCommand::GetPossibleNameForService(const std::string &service) {
    auto possible_names = vector<string>{service};
    std::filesystem::path service_path{service};
    if (auto ext = service_path.extension();
        ext != "" && ext != (".system") && ext != ".user") {
        possible_names.push_back(service + ".system");
        possible_names.push_back(service + ".user");
    }
    return possible_names;
}
