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

#include <memory>
#include <vector>

#include "bitsery/ext/growable.h"
#include "bitsery/ext/std_variant.h"

#include "tt/data/service.hpp"

namespace tt {

class ServiceNode;

class ServiceNode {
public:
    explicit ServiceNode(Service service);
    auto operator==(const ServiceNode &node) const -> bool;
    [[nodiscard]] auto service() const -> Service;
    [[nodiscard]] auto name() const -> const std::string &;

    void AddDependant();
    [[nodiscard]] auto HasDependants() const -> bool;
    void RemoveDependant();

    void Dump(std::ostream &oss) const;

private:
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
        serializer.value4b(dependants_);
    }
    Service service_;
    std::string service_name_;
    uint32_t dependants_{0};
};

} // namespace tt

auto operator<<(std::ostream &oss, const tt::ServiceNode &node)
    -> std::ostream &;
