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

#include "args.hxx"

#include "tt/cli/command/command.hpp"

namespace tt::cli {

class ShowCommand : public Command {
public:
    ShowCommand(args::Subparser &parser,
                std::shared_ptr<GlobalOptions> common_options);

    static constexpr inline const char *name = "show";
    static constexpr inline const char *description = "Show info of a service";

protected:
    auto Execute() -> int override;

private:
    args::Positional<std::string> service_;
};

} // namespace tt::cli
