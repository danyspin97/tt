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

#include "libtt/data/environment.hpp"

using std::map;
using std::ostream;
using std::string;

using tt::Environment;

const string Environment::Get(const string key) const { return env_.at(key); }

const map<string, string> Environment::GetAll() const { return env_; }

void Environment::Set(const string key, const string value) {
    env_[key] = value;
}
bool Environment::SetUnique(const string key, const string value) {
    if (env_.find(key) == env_.end()) {
        return false;
    }

    env_[key] = value;
    return true;
}

ostream &operator<<(ostream &strm, const Environment &env) {
    auto env_map = env.GetAll();
    for (const auto &pair : env_map) {
        strm << pair.first << " = \"" << pair.second << "\"\n";
    }
    return strm;
}
