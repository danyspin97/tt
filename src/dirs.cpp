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

#include "tt/dirs.hpp"

tt::Dirs &tt::Dirs::GetInstance() {
    static tt::Dirs instance;

    return instance;
}

const std::string &tt::Dirs::bindir() { return bindir_; }

const std::string &tt::Dirs::confdir() { return confdir_; }

const std::string &tt::Dirs::execline_prefix() { return execline_prefix_; }

const std::string &tt::Dirs::libexecdir() { return libexecdir_; }

const std::string &tt::Dirs::livedir() { return livedir_; }

const std::string &tt::Dirs::logdir() { return logdir_; }

const std::string &tt::Dirs::servicedir() { return servicedir_; }

const std::string &tt::Dirs::statedir() { return statedir_; }
