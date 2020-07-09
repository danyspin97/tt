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

#include <utility>

#include "tt/data/script.hpp"

using std::ostream;

using tt::Script;

Script::Script(Type type, std::string &&execute)
    : execute_(std::move(execute)), type_(type) {}

Script::Script(Type type, std::string &&execute, std::string &&user,
               std::string &&group)
    : execute_(std::move(execute)), type_(type), user_(std::move(user)),
      group_(std::move(group)) {}

auto Script::Dump(ostream &oss) const -> ostream & {
    // TODO: Convert from Type to string
    // o << "type = " << type() << "\n";
    oss << "execute = (\n" << execute() << "\n)";
    if (user() && !user().value().empty()) {
        oss << "\nuser = " << user().value();
    }
    if (group() && !group().value().empty()) {
        oss << "\ngroup = " << group().value();
    }
    oss << "\ntimeout_finish = " << timeout();
    oss << "\ntimeout_kill = " << timeout_kill();
    oss << "\ndown_signal = " << GetSignalName(down_signal());
    oss << "\nmax_death = " << max_death();
    return oss;
}

auto operator<<(ostream &oss, const Script &script) -> ostream & {
    return script.Dump(oss);
}
