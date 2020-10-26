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

#include <string> // for string, allocator

#include "args.hxx" // for ValueFlag, Flag, Group, HelpFlag

namespace tt::cli {

class GlobalOptions {
public:
    GlobalOptions() = default;

    auto arguments() -> args::Group &;
    auto help() const -> bool;
    auto quiet() const -> bool;
    auto verbosity() -> args::ValueFlag<std::string> &;

private:
    args::Group arguments_{"Global options:"};
    args::HelpFlag help_{
        arguments_,
        "help",
        "Display this help menu",
        {'h', "help"},
    };
    args::Flag quiet_{
        arguments_, "quiet", "Do not print output.", {'q', "quiet"}};
    args::ValueFlag<std::string> verbosity_{
        arguments_,
        "verbosity",
        "Set the verbosity level to one of {'debug', 'info', 'warn', "
        "'critical', 'err'}",
        {"verbosity"}};
};

} // namespace tt::cli
