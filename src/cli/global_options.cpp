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

#include "tt/cli/global_options.hpp"

using tt::cli::GlobalOptions;

GlobalOptions::GlobalOptions()
    : arguments_("Global options:"),
      help_(arguments_, "help", "Display this help menu", {'h', "help"}),
      quiet_(arguments_, "quiet", "Do not print output.", {'q', "quiet"}),
      verbosity_(arguments_, "verbosity",
                 "Set the verbosity level from 0 (Error) to 4 (Trace)",
                 {"verbosity"}) {}
