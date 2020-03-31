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

#ifndef LIBTT_LOGGER_SCRIPT_HPP_
#define LIBTT_LOGGER_SCRIPT_HPP_

#include <string>

#include "libtt/data/environment.hpp"
#include "libtt/data/script.hpp"

namespace tt {

class LoggerScript : public Script {
public:
    const std::string service_to_log() const noexcept {
        return service_to_log_;
    }

    LoggerScript(Type type, const std::string execute, Environment environment,
                 const std::string service_to_log, const std::string user,
                 const std::string group)
        : Script(type, execute, environment) {
        service_to_log_ = service_to_log;
        this->user(user);
        this->group(group);
    }

    std::ostream &dump(std::ostream &oss) const {
        Script::dump(oss);
        return oss << "\nservice_to_log = " << service_to_log();
    }

private:
    std::string service_to_log_;
};

} // namespace tt

#endif // LIBTT_LOGGER_SCRIPT_HPP_