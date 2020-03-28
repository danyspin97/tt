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

#ifndef LIBTT_ONESHOT_HPP_
#define LIBTT_ONESHOT_HPP_

#include <optional>

#include "libtt/data/oneshot_options.hpp"
#include "libtt/data/script.hpp"
#include "libtt/data/service.hpp"

namespace tt {

class Oneshot : public Service {
public:
    Script start() const noexcept { return start_; }

    // set method for start_ as it is required

    std::optional<Script> stop() const noexcept { return stop_; }

    void stop(Script stop) { stop_ = stop; }

    Oneshot(const std::string name, const std::string polish_name,
            const std::string description, const std::string path,
            OneshotOptions options, Script start)
        : Service(name, polish_name, description, path, options),
          start_(start) {}

    std::ostream &dump(std::ostream &oss) const {
        oss << "[main]\n";
        Service::dump(oss);
        oss << "\ntype = oneshot";
        oss << "\n\n[start]\n" << start_;
        if (stop_) {
            oss << "\n\n[stop]\n" << stop().value();
        }
        oss << "\n\n[options]\n" << options();
        if (start_.environment().getAll().size() != 0) {
            oss << "\n\n[config]\n" << start_.environment();
        }
        return oss;
    }

private:
    Script start_;
    std::optional<Script> stop_;
};

} // namespace tt

#endif // LIBTT_ONESHOT_HPP_
