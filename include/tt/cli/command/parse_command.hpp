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

#include <memory> // for shared_ptr
#include <string> // for string
#include <vector> // for vector

#include "args.hxx" // for Flag, PositionalList, Subparse...

#include "tt/cli/command/command.hpp" // for Command

namespace tt::cli {
class GlobalOptions;
} // namespace tt::cli

namespace tt::cli {

class ParseCommand : public Command {
public:
    explicit ParseCommand(args::Subparser &parser);

    static constexpr inline const char *name = "parse";
    static constexpr inline const char *description =
        "Parse one or more services for testing purposes.";

protected:
    auto Execute() -> int override;

private:
    void ParseFiles() const;
    void ParseUserSystemServices();
    auto ParseForFileInDefaultDirs(const ::std::string &name) const -> bool;
    static auto GetPossibleNameForService(const std::string &service)
        -> std::vector<std::string>;

    args::PositionalList<std::string> service_list_;
    args::Flag is_file_;
};

} // namespace tt::cli
