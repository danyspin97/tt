/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#ifndef TT_ONESHOT_HPP_
#define TT_ONESHOT_HPP_

#include <optional>

#include "tt/data/oneshot_options.hpp"
#include "tt/data/script.hpp"
#include "tt/data/service.hpp"

namespace tt {

class ServiceVisitor;

class Oneshot : public Service {
public:
    Script start() const noexcept { return start_; }

    // set method for start_ as it is required

    std::optional<Script> stop() const noexcept { return stop_; }

    void stop(const Script &stop) { stop_.emplace(stop); }

    Oneshot(const std::string &name, const std::string &polish_name,
            const std::string &description, const std::string &path,
            OneshotOptions options, const Script &start);

    std::ostream &Dump(std::ostream &oss) const;
    void Accept(ServiceVisitor &visitor);

private:
    Script start_;
    std::optional<Script> stop_;
};

} // namespace tt

#endif // TT_ONESHOT_HPP_
