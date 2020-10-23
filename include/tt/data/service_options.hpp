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

#ifndef TT_SERVICE_OPTIONS_HPP_
#define TT_SERVICE_OPTIONS_HPP_

#include <istream>
#include <string>
#include <utility>
#include <vector>

#include "bitsery/traits/string.h"
#include "bitsery/traits/vector.h"

namespace tt {

class ServiceOptions {
public:
    [[nodiscard]] auto dependencies() const -> std::vector<std::string> {
        return depends_;
    }

    void dependencies(std::vector<std::string> depends) {
        depends_ = std::move(depends);
    }

    virtual ~ServiceOptions() = default;

    virtual auto Dump(std::ostream &oss) const -> std::ostream &;

protected:
    friend class ServiceImpl;
    ServiceOptions() = default;

private:
    friend class bitsery::Access;
    template <typename S> void serialize(S &serializer) {
        serializer.container(
            depends_, depends_.max_size(),
            [](S &serializer, std::string &dependency) {
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(dependency,
                                                      dependency.max_size());
            });
    }

    std::vector<std::string> depends_;
};

} // namespace tt

auto operator<<(std::ostream &oss, tt::ServiceOptions const &options)
    -> std::ostream &;

#endif // TT_SERVICE_OPTIONS_HPP_
