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

#include <functional>  // for function
#include <memory>      // for shared_ptr
#include <type_traits> // for is_base_of

namespace args {
class Subparser;
}

namespace tt::cli {

class Command;
class GlobalOptions;

class CommandDispatcher {
public:
    static void
    SetGlobalOptions(std::shared_ptr<GlobalOptions> global_options) {
        global_options_ = std::move(global_options);
    }

    template <typename T>
    static auto Dispatch() -> std::function<void(args::Subparser &)> {
        static_assert(std::is_base_of_v<Command, T>,
                      "Dispatch only works with a subclass of Command");
        return [](args::Subparser &subparser) {
            T command = T(subparser, std::move(global_options_));
            exit_code_ = command.InitAndExecute();
        };
    }

    [[nodiscard]] static auto exit_code() -> int { return exit_code_; }

private:
    static inline int exit_code_ = -1;
    static inline std::shared_ptr<GlobalOptions> global_options_;
};

} // namespace tt::cli
