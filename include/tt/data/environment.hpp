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

#ifndef TT_ENVINRONMENT_HPP_
#define TT_ENVINRONMENT_HPP_

#include <istream>
#include <map>
#include <string>

#include "bitsery/ext/std_tuple.h"
#include "bitsery/traits/string.h"
#include "bitsery/traits/vector.h"

namespace tt {

class Environment {
public:
    [[nodiscard]] auto Get(const std::string &key) const -> std::string;
    [[nodiscard]] auto GetKeys() const -> std::vector<std::string>;
    void Set(const std::string &key, const std::string &value);
    auto SetUnique(const std::string &key, const std::string &value) -> bool;
    [[nodiscard]] auto CountKeys() const -> int;
    void UpdateValuesWithEnvironment(const Environment &env);
    [[nodiscard]] auto Vector() const -> std::vector<std::string>;

    auto Dump(std::ostream &strm) const -> std::ostream &;

private:
    friend class bitsery::Access;
    template <typename S> void serialize(S &serializer) {
        serializer.container(
            pairs_, pairs_.max_size(),
            [](S &serializer, std::tuple<std::string, std::string> &pair) {
                serializer.ext(
                    pair, bitsery::ext::StdTuple{[](S &serializer,
                                                    std::string &value) {
                        serializer.template text<
                            sizeof(std::string::value_type), std::string>(
                            value, value.max_size());
                    }});
            });
    }

    std::vector<std::tuple<std::string, std::string>> pairs_;
};

} // namespace tt

auto operator<<(std::ostream &strm, const tt::Environment &env)
    -> std::ostream &;

#endif // TT_ENVINRONMENT_HPP_
