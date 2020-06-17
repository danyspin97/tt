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

#ifndef TT_DATA_SCRIPT_HPP_
#define TT_DATA_SCRIPT_HPP_

#include <cassert>
#include <optional>
#include <string>

#include "bitsery/ext/std_optional.h"
#include "bitsery/traits/string.h"

#include "tt/data/environment.hpp"

namespace tt {

class Script {
public:
    enum class Type : uint16_t {
        Bash,
        Execline,
    };

    [[nodiscard]] Type type() const noexcept { return type_; }
    [[nodiscard]] std::string execute() const noexcept { return execute_; }
    [[nodiscard]] std::optional<std::string> user() const noexcept {
        return user_;
    }
    void user(const std::string &user) noexcept { user_ = user; }
    [[nodiscard]] std::optional<std::string> group() const noexcept {
        return group_;
    }
    void group(const std::string &group) noexcept { group_ = group; }
    Script(Type type, std::string execute);
    virtual ~Script() = default;
    virtual std::ostream &Dump(std::ostream &oss) const;

protected:
    friend class Longrun;
    friend class Oneshot;
    // Needed for LoggerScript()
    Script() = default;

private:
    friend class bitsery::Access;
    template <typename S> void serialize(S &serializer) {
        serializer.template text<sizeof(std::string::value_type), std::string>(
            execute_, execute_.max_size());
        serializer.value2b(type_);
        serializer.ext(
            user_, bitsery::ext::StdOptional{true},
            [](S &serializer, std::string &user) {
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(user, user.max_size());
            });
        serializer.ext(
            user_, bitsery::ext::StdOptional{true},
            [](S &serializer, std::string &group) {
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(group, group.max_size());
            });
    }

protected:
    std::string execute_;

private:
    Type type_;
    std::optional<std::string> user_;
    std::optional<std::string> group_;
};

} // namespace tt

std::ostream &operator<<(std::ostream &oss, const tt::Script &script);

#endif // TT_DATA_SCRIPT_HPP_
