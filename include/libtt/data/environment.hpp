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

#ifndef LIBTT_ENVINRONMENT_HPP_
#define LIBTT_ENVINRONMENT_HPP_

#include <istream>
#include <map>
#include <string>

namespace tt {

class Environment {
public:
    const std::string get(const std::string key) const { return env_.at(key); }

    const std::map<std::string, std::string> getAll() const { return env_; }

    void set(const std::string key, const std::string value) {
        env_[key] = value;
    }

    bool setUnique(const std::string key, const std::string value) {
        if (env_.find(key) == env_.end()) {
            return false;
        }

        env_[key] = value;
        return true;
    }

private:
    std::map<std::string, std::string> env_;
};

std::ostream &operator<<(std::ostream &strm, const Environment &env) {
    auto env_map = env.getAll();
    for (const auto &pair : env_map) {
        strm << pair.first << " = \"" << pair.second << "\"\n";
    }
    return strm;
}

} // namespace tt

#endif // LIBTT_ENVINRONMENT_HPP_
