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

#include <string> // for string

#include "msgpack.hpp"

#include "tt/action/action.hpp" // for Action

namespace tt {

class NotifyUpAction : public Action {
public:
    explicit NotifyUpAction(std::string service, bool succeded);
    NotifyUpAction() = delete;

    [[nodiscard]] auto service() const -> std::string;
    [[nodiscard]] auto succeded() const -> bool;

    void Apply() override;

    MSGPACK_DEFINE_MAP(MSGPACK_BASE_MAP(Action), service_, succeeded_)

private:
    std::string service_;
    bool succeeded_;
};

} // namespace tt
