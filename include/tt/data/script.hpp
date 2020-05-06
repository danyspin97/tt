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

#include <optional>
#include <string>

#include "tt/data/environment.hpp"

namespace tt {

class Script {
public:
    enum class Type {
        Bash,
        Execline,
    };

    Type type() const noexcept { return type_; }
    std::string execute() const noexcept { return execute_; }
    std::optional<std::string> user() const noexcept { return user_; }
    void user(const std::string &user) noexcept { user_ = user; }
    std::optional<std::string> group() const noexcept { return group_; }
    void group(const std::string &group) noexcept { group_ = group; }
    Environment environment() const noexcept { return env_; }
    Script(Type type, const std::string &execute, Environment &environment);
    virtual ~Script() = default;
    virtual std::ostream &Dump(std::ostream &oss) const;

protected:
    std::string execute_;

private:
    Type type_;
    std::optional<std::string> user_;
    std::optional<std::string> group_;
    Environment &env_;
};

} // namespace tt

std::ostream &operator<<(std::ostream &oss, const tt::Script &script);

#endif // TT_DATA_SCRIPT_HPP_
