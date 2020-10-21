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

#include <memory>

#include "tt/cli/global_options.hpp"

namespace args {
class Subparser;
}

namespace tt {
class CliLogger;
}

namespace tt::cli {

class Command {
public:
    Command(args::Subparser &parser,
            std::shared_ptr<GlobalOptions> global_options);
    virtual ~Command() = default;

    auto InitAndExecute() -> int;

    template <typename T>
    static auto Dispatch(args::Subparser &parser,
                         std::shared_ptr<GlobalOptions> common_options) -> int {
        static_assert(std::is_base_of<Command, T>::value,
                      "Dispatch only works with a subclass of Command");
        T command = T(parser, std::move(common_options));
        return command.InitAndExecute();
    }

protected:
    virtual auto Execute() -> int = 0;

    [[nodiscard]] auto logger() const -> std::shared_ptr<CliLogger>;

    args::Subparser &parser_;
    std::shared_ptr<GlobalOptions> global_options_;

private:
    std::shared_ptr<CliLogger> logger_;
};

} // namespace tt::cli
