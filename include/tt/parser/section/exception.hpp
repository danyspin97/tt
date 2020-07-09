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

#ifndef TT_SECTION_EXCEPTION_HPP_
#define TT_SECTION_EXCEPTION_HPP_

#include <string>

#include "tt/exception.hpp"

namespace tt {

class AttributeIsAlreadySetException : public Exception {
public:
    explicit AttributeIsAlreadySetException(const std::string &msg)
        : Exception(msg) {}
};

class AttributeNotFoundException : public Exception {
public:
    explicit AttributeNotFoundException(const std::string &msg)
        : Exception(msg) {}
};

class CodeNotSuppliedInScriptParserException : public Exception {
public:
    explicit CodeNotSuppliedInScriptParserException(const std::string &msg)
        : Exception(msg) {}
};

class DummyBuilderException : public Exception {
public:
    DummyBuilderException() : Exception("") {}
};

class EnvironmentKeyNotValidException : public Exception {
public:
    explicit EnvironmentKeyNotValidException(const std::string &msg)
        : Exception(msg) {}
};

class LoggerScriptInvalidSettingsException : public Exception {
public:
    explicit LoggerScriptInvalidSettingsException(const std::string &msg)
        : Exception(msg) {}
};

class ParamIsNotEmptyException : public Exception {
public:
    ParamIsNotEmptyException() : Exception("") {}
};

class ScriptTypeNotValidException : public Exception {
public:
    explicit ScriptTypeNotValidException(const std::string &msg)
        : Exception(msg) {}
};

class SectionBuilderException : public Exception {
public:
    explicit SectionBuilderException(const std::string &msg) : Exception(msg) {}
};

} // namespace tt

#endif // TT_SECTION_EXCEPTION_HPP_
