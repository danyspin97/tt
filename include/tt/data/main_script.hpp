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

#include <iosfwd> // for ostream

#include "bitsery/ext/inheritance.h" // for BaseClass

#include "tt/data/script.hpp" // for Script

namespace bitsery {
class Access;
} // namespace bitsery

namespace tt {

class MainScript : public Script {
public:
    using Script::Script;

    auto operator==(const MainScript &rhs) const -> bool;
    auto operator!=(const MainScript &rhs) const { return !operator==(rhs); }

    [[nodiscard]] auto autostart() const noexcept -> bool { return autostart_; }
    void autostart(bool autostart) { autostart_ = autostart; }

    auto Dump(std::ostream &oss) const -> std::ostream & override;

protected:
    friend class Longrun;
    friend class Oneshot;
    MainScript() = default;

private:
    friend class bitsery::Access;
    template <typename S> void serialize(S &serializer) {
        serializer.ext(*this, bitsery::ext::BaseClass<Script>{});
        serializer.value1b(autostart_);
    }

    bool autostart_ = true;
};

} // namespace tt
