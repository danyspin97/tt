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

#include <memory>   // for shared_ptr
#include <unistd.h> // for geteuid

#include "tt/path/user_dirs.hpp" // for UserDirs

namespace args {
class Subparser;
}

namespace tt {
class CliLogger;

namespace cli {
class GlobalOptions;
} // namespace cli
} // namespace tt

namespace tt::cli {

class Command {
public:
    Command(args::Subparser &parser,
            std::shared_ptr<GlobalOptions> global_options);
    virtual ~Command() = default;

    auto Run(const std::string &name) -> int;

protected:
    virtual auto Execute() -> int = 0;

    [[nodiscard]] auto logger() const -> std::shared_ptr<CliLogger>;
    [[nodiscard]] auto dirs() const -> std::shared_ptr<Dirs>;

private:
    void Init(const std::string &name);

protected:
    args::Subparser &parser_;
    std::shared_ptr<GlobalOptions> global_options_;

private:
    std::shared_ptr<CliLogger> logger_;
    std::shared_ptr<Dirs> dirs_ = geteuid() > 0
                                      ? std::make_shared<UserDirs>()
                                      : std::make_shared<SystemDirs>();
};

} // namespace tt::cli
