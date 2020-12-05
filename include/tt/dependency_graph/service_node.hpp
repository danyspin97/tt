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

#pragma once

#include <cstdint> // for uint32_t
#include <iosfwd>  // for ostream
#include <string>  // for string
#include <variant> // for monostate

#include "bitsery/ext/growable.h"    // for Growable
#include "bitsery/ext/std_variant.h" // IWYU pragma: keep for StdVariant

#include "tt/data/service.hpp" // for Service

namespace bitsery {
class Access;
} // namespace bitsery

namespace tt {
class Bundle;
class Longrun;
class Oneshot;

class ServiceNode {
public:
    explicit ServiceNode(Service service);
    auto operator==(const ServiceNode &rhs) const -> bool;
    auto operator!=(const ServiceNode &rhs) const -> bool {
        return !operator==(rhs);
    }

    [[nodiscard]] auto service() const -> const Service &;
    [[nodiscard]] auto name() const -> const std::string &;
    [[nodiscard]] auto dependants() const -> const std::vector<std::string> &;

    void AddDependant(const std::string &dependant_name);
    [[nodiscard]] auto HasDependants() const -> bool;
    void RemoveDependant(const std::string &dependant_name);

    void Dump(std::ostream &oss) const;

private:
    friend class LiveService;
    friend class bitsery::Access;
    ServiceNode() = default;
    template <typename S> void serialize(S &serializer) {
        serializer.template text<sizeof(std::string::value_type), std::string>(
            service_name_, service_name_.max_size());
        serializer.ext(
            service_,
            bitsery::ext::StdVariant{
                [](S & /*unused*/, std::monostate & /*unused*/) {},
                [](S &serializer, Bundle &bundle) {
                    serializer.ext(bundle, bitsery::ext::Growable{},
                                   [](S &serializer, Bundle &bundle) {
                                       serializer.object(bundle);
                                   });
                },
                [](S &serializer, Longrun &longrun) {
                    serializer.ext(longrun, bitsery::ext::Growable{},
                                   [](S &serializer, Longrun &longrun) {
                                       serializer.object(longrun);
                                   });
                },
                [](S &serializer, Oneshot &oneshot) {
                    serializer.ext(oneshot, bitsery::ext::Growable{},
                                   [](S &serializer, Oneshot &oneshot) {
                                       serializer.object(oneshot);
                                   });
                }});
        serializer.container(
            dependants_, dependants_.max_size(),
            [](S &serializer, std::string &value) {
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(value, value.max_size());
            });
    }
    Service service_;
    std::string service_name_;
    std::vector<std::string> dependants_;
};

} // namespace tt

auto operator<<(std::ostream &oss, const tt::ServiceNode &node)
    -> std::ostream &;
