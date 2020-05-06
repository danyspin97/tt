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

#ifndef TT_ENVINRONMENT_HPP_
#define TT_ENVINRONMENT_HPP_

#include <istream>
#include <map>
#include <string>

namespace tt {

class Environment {
public:
    std::string Get(const std::string &key) const;

    std::map<std::string, std::string> GetAll() const;

    void Set(const std::string &key, const std::string &value);

    bool SetUnique(const std::string &key, const std::string &value);

private:
    std::map<std::string, std::string> env_;
};

} // namespace tt

std::ostream &operator<<(std::ostream &strm, const tt::Environment &env);

#endif // TT_ENVINRONMENT_HPP_
