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

Script::Script(Type type, std::string execute, Environment &environment)
    : execute_(std::move(execute)), type_(type), env_(environment) {}

ostream &Script::Dump(ostream &oss) const {
    // TODO: Convert from Type to string
    // o << "type = " << type() << "\n";
    oss << "execute = (\n" << execute() << "\n)";
    if (user() && !user().value().empty()) {
        oss << "\nuser = " << user().value();
    }
    if (group() && !group().value().empty()) {
        oss << "\ngroup = " << group().value();
    }
    return oss;
}

ostream &operator<<(ostream &oss, const Script &script) {
    return script.Dump(oss);
}
