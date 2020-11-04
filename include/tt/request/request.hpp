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

#include <string>

#include "msgpack.hpp"

namespace tt {

class Request {
public:
    virtual ~Request() = default;
    virtual void Apply() = 0;

    [[nodiscard]] auto name() const noexcept -> const std::string & {
        return name_;
    }

    MSGPACK_DEFINE_MAP(name_)

protected:
    void name(std::string name) noexcept { name_ = std::move(name); }

private:
    std::string name_;
};

} // namespace tt
