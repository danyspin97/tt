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

#include <iosfwd>   // for ostream
#include <optional> // for optional
#include <stdint.h> // for uint_fast32_t

#include "bitsery/ext/inheritance.h"  // for BaseClass
#include "bitsery/ext/std_optional.h" // for StdOptional

#include "tt/data/main_script.hpp" // for MainScript

namespace bitsery {
class Access;
} // namespace bitsery

namespace tt {

class LongLivedScript : public MainScript {
public:
    using MainScript::MainScript;

    [[nodiscard]] auto notify() const noexcept -> std::optional<uint_fast32_t> {
        return notify_;
    }
    void notify(uint_fast32_t notify) noexcept { notify_ = notify; }

    auto Dump(std::ostream &oss) const -> std::ostream & override;

protected:
    friend class Longrun;
    friend class Oneshot;
    LongLivedScript() = default;

private:
    friend class bitsery::Access;
    template <typename S> void serialize(S &serializer) {
        serializer.ext(*this, bitsery::ext::BaseClass<MainScript>{});
        serializer.ext(notify_, bitsery::ext::StdOptional{true},
                       [](S &serializer, uint_fast32_t notify) {
                           serializer.template value<sizeof(notify)>(notify);
                       });
    }

    std::optional<uint_fast32_t> notify_;
};

} // namespace tt
