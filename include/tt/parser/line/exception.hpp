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

#ifndef TT_PARSER_LINE_EXCEPTION_HPP_
#define TT_PARSER_LINE_EXCEPTION_HPP_

#include <string>

#include "tt/exception.hpp"

namespace tt {

class CodeParserIsStillParsingException : public Exception {
public:
    CodeParserIsStillParsingException()
        : Exception("Code parser is still parsing") {}
};

class EmptyKeyException : public Exception {
public:
    EmptyKeyException() : Exception("Empty keys are not allowed") {}
};

class EmptyArrayException : public Exception {
public:
    explicit EmptyArrayException(const std::string &msg) : Exception(msg) {}
};

class KeyValueParserLineInvalidException : public Exception {
public:
    explicit KeyValueParserLineInvalidException(const std::string &msg)
        : Exception(msg) {}
};

class ValuesAfterEndingTokenException : public Exception {
public:
    explicit ValuesAfterEndingTokenException(const std::string &msg) : Exception(msg) {}
};

} // namespace tt

#endif // TT_PARSER_LINE_EXCEPTION_HPP_
