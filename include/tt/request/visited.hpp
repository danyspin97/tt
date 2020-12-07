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

#include <memory> // for shared_ptr, enable_shared_from_this

#include "nlohmann/json.hpp" // for json

#include "tl/expected.hpp" // for expected

#include "tt/request/reply/reply.hpp" // for Reply
#include "tt/request/request.hpp"     // for Request
#include "tt/request/visitor.hpp"     // for Visitor

namespace tt::request {

template <typename T>
class Visited : public Request, public std::enable_shared_from_this<T> {
public:
    auto accept(Visitor &visitor)
        -> tl::expected<nlohmann::json, std::string> override {
        return visitor(this->shared_from_this());
    }
};

} // namespace tt::request
