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

#ifndef LIBTT_SECTION_EXCEPTION_HPP_
#define LIBTT_SECTION_EXCEPTION_HPP_

#include <string>

#include "libtt/exception.hpp"

namespace tt {

class AttributeNotFoundException : public Exception {
public:
    AttributeNotFoundException(const std::string &msg) : Exception(msg) {}
};

class CodeNotSuppliedInScriptParserException : public Exception {
public:
    CodeNotSuppliedInScriptParserException(const std::string &msg)
        : Exception(msg) {}
};

class LoggerScriptInvalidSettingsException : public Exception {
public:
    LoggerScriptInvalidSettingsException(const std::string &msg)
        : Exception(msg) {}
};

class ScriptTypeNotValidException : public Exception {
public:
    ScriptTypeNotValidException(const std::string &msg) : Exception(msg) {}
};

} // namespace tt

#endif // LIBTT_SECTION_EXCEPTION_HPP_
