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

#include "tt/parser/section/environment_builder.hpp"

#include <cctype>

#include "tt/parser/line/exception.hpp"
#include "tt/parser/line/key_value_parser.hpp"
#include "tt/parser/section/exception.hpp"
#include "tt/parser/section/utils.hpp"

using std::string;

using tt::Environment;
using tt::EnvironmentBuilder;
using tt::EnvironmentKeyNotValidException;
using tt::EnvironmentValueNotQuotedException;
using tt::IsEmptyLine;
using tt::KeyValueParser;
using tt::KeyValueParserLineInvalidException;
using tt::SectionBuilderException;

EnvironmentBuilder::EnvironmentBuilder(Environment &environment)
    : environment_(environment) {}

void EnvironmentBuilder::ParseLine(const string &line) {
    try {
        TryParseLine(line);
    } catch (KeyValueParserLineInvalidException &e) {
        const string msg = e.msg() + " in [config] section";
        throw SectionBuilderException(msg);
    } catch (EnvironmentKeyNotValidException &e) {
        const string msg = e.msg() + " in [config] section";
        throw SectionBuilderException(msg);
    }
}

void EnvironmentBuilder::TryParseLine(const string &line) {
    if (IsEmptyLine(line)) {
        return;
    }

    auto key_value_parser = KeyValueParser(line, true);
    const string key = key_value_parser.key();
    CheckKeyIsValid(key);
    const string value = StripQuotes(key_value_parser.value());

    environment_.Set(key, value);
}

void EnvironmentBuilder::CheckKeyIsValid(const string &key) {
    if (isdigit(key[0]) != 0) {
        const auto msg = "Key " + key + " cannot start with a digit";
        throw EnvironmentKeyNotValidException(msg);
    }

    for (char c : key) {
        if ((isalnum(c) == 0) && c != '_') {
            const auto msg =
                "Character " + string{c} + " not valid in key " + key;
            throw EnvironmentKeyNotValidException(msg);
        }
    }
}

string EnvironmentBuilder::StripQuotes(const string &value) {
    const auto val_size = value.size();
    if (val_size < 2 || value[0] != '"' || value[val_size - 1] != '"') {
        throw EnvironmentValueNotQuotedException();
    }

    return value.substr(1, val_size - 2);
}
