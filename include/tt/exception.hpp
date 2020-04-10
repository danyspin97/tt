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

#ifndef TT_EXCEPTION_HPP_
#define TT_EXCEPTION_HPP_

namespace tt {

class Exception : public std::exception {
public:
    Exception(const std::string msg) : msg_(msg) {}

    const char *what() const noexcept { return msg_.c_str(); }

private:
    const std::string msg_;
};

class SignalNotValidExecption : public Exception {
public:
    SignalNotValidExecption(const std::string msg) : Exception(msg) {}
};

} // namespace tt

#endif // TT_EXCEPTION_HPP_
