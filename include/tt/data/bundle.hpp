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

#ifndef TT_BUNDLE_HPP_
#define TT_BUNDLE_HPP_

#include <istream> // for ostream
#include <string>  // for string

#include "bitsery/ext/inheritance.h" // for BaseClass

#include "tt/data/bundle_options.hpp" // for BundleOptions
#include "tt/data/service_impl.hpp"   // for ServiceImpl

namespace bitsery {
class Access;
} // namespace bitsery

namespace tt {

class Bundle : public ServiceImpl {
public:
    Bundle(std::string &&name, std::string &&description, std::string &&path,
           BundleOptions &&options);

    [[nodiscard]] auto options() const noexcept -> BundleOptions {
        return options_;
    }

    auto Dump(std::ostream &oss) const -> std::ostream & override;

private:
    friend class bitsery::Access;
    Bundle() = default;

    template <typename S> void serialize(S &serializer) {
        serializer.ext(*this, bitsery::ext::BaseClass<ServiceImpl>{});
        serializer.object(options_);
    }

    BundleOptions options_;
};

} // namespace tt

auto operator<<(std::ostream &oss, const tt::Bundle &bundle) -> std::ostream &;

#endif // TT_BUNDLE_HPP_
