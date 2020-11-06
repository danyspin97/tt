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
    CommandDispatcher() = default;

    template <typename T>
    auto Dispatch() -> std::function<void(args::Subparser &)> {
        static_assert(std::is_base_of_v<Command, T>,
                      "Dispatch only works with a subclass of Command");
        return [this](args::Subparser &subparser) {
            T command = T(subparser);
            exit_code_ = command.Run(T::name);
        };
    }

    [[nodiscard]] auto exit_code() const -> int { return exit_code_; }

private:
    int exit_code_ = -1;
};

} // namespace tt::cli
