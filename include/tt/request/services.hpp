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

#include "tt/request/request.hpp" // for Request
#include "tt/request/visited.hpp" // for Visited

namespace tt::request {

class Services {
public:
    explicit Services(std::vector<std::string> services)
        : services_(std::move(services)) {}
    Services() = delete;

    [[nodiscard]] auto services() const -> std::vector<std::string> {
        return services_;
    }
    [[nodiscard]] auto services() -> std::vector<std::string> && {
        return std::move(services_);
    }

private:
    std::vector<std::string> services_;
};

} // namespace tt::request
