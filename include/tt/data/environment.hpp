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

#include "bitsery/ext/std_map.h"

namespace tt {

class Environment {
public:
    [[nodiscard]] auto Get(const std::string &key) const -> std::string;

    [[nodiscard]] auto GetAll() const -> std::map<std::string, std::string>;

    void Set(const std::string &key, const std::string &value);

    auto SetUnique(const std::string &key, const std::string &value) -> bool;

private:
    friend class bitsery::Access;

    template <typename S> void serialize(S &serializer) {
        serializer.ext(
            env_, bitsery::ext::StdMap{env_.max_size()},
            [](S &serializer, std::string &key, std::string &value) {
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(key, key.max_size());
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(value, value.max_size());
            });
    }

    std::map<std::string, std::string> env_;
};

} // namespace tt

auto operator<<(std::ostream &strm, const tt::Environment &env)
    -> std::ostream &;

#endif // TT_ENVINRONMENT_HPP_
