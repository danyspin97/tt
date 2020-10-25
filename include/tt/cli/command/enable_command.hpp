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

#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "args.hxx"

#include "tt/cli/command/command.hpp"
#include "tt/parser/service/services_parser.hpp"

namespace tt::cli {

class EnableCommand : public Command {
public:
    EnableCommand(args::Subparser &parser,
                  std::shared_ptr<GlobalOptions> common_options);
    ~EnableCommand() override = default;

protected:
    auto Execute() -> int override;

private:
    auto EnableServices() -> int;
    auto GetGraphPath() -> std::filesystem::path;

    args::PositionalList<std::string> service_list_;
    // TODO: Add more arguments
};

} // namespace tt::cli
