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

#ifndef TT_LOGGER_SCRIPT_HPP_
#define TT_LOGGER_SCRIPT_HPP_

#include <string>

#include "bitsery/ext/inheritance.h"

#include "tt/data/environment.hpp"
#include "tt/data/script.hpp"

namespace tt {

class LoggerScript : public Script {
public:
    [[nodiscard]] auto service_to_log() const noexcept -> std::string {
        return service_to_log_;
    }

    LoggerScript(Type type, const std::string &execute,
                 std::string service_to_log, const std::string &user,
                 const std::string &group);

    auto Dump(std::ostream &oss) const -> std::ostream & override;

private:
    friend class bitsery::Access;
    LoggerScript() = default;
    template <typename S> void serialize(S &serializer) {
        serializer.ext(*this, bitsery::ext::BaseClass<Script>{});
        serializer.template text<sizeof(std::string::value_type), std::string>(
            service_to_log_, service_to_log_.max_size());
    }

    std::string service_to_log_;
};

} // namespace tt

#endif // TT_LOGGER_SCRIPT_HPP_
