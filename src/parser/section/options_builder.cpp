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

#include "tt/parser/section/options_builder.hpp"

#include "tt/parser/exception.hpp"
#include "tt/parser/line/exception.hpp"
#include "tt/parser/line/key_value_parser.hpp"
#include "tt/parser/section/exception.hpp"
#include "tt/parser/section/utils.hpp"

using std::string;

using tt::AttributeNotFoundException;
using tt::BooleanParseException;
using tt::EmptyArrayException;
using tt::EmptyKeyException;
using tt::IsEmptyLine;
using tt::KeyValueParser;
using tt::OptionsBuilder;
using tt::SectionBuilderException;
using tt::ValuesAfterEndingTokenException;

void OptionsBuilder::ParseLine(const string &line) {
    if (IsEmptyLine(line)) {
        return;
    }

    if (ParseMultilineValue(line)) {
        return;
    }

    auto key_value_parser = KeyValueParser(line);
    if (key_value_parser.IsLineValid()) {
        const string key = key_value_parser.key();
        const string value = key_value_parser.value();
        SetAttributeForKey(key, value);
        return;
    }

    throw SectionBuilderException(line + "\n is not valid");
}

void OptionsBuilder::EndParsing() {
    if (array_parser_.IsParsing()) {
        throw SectionBuilderException(
            array_parser_.key() + " has not been closed in section [options]");
    }
}

auto OptionsBuilder::ParseMultilineValue(const string &line) -> bool {
    try {
        return TryParseMultilineValue(line);
    } catch (AttributeNotFoundException &e) {
        throw SectionBuilderException(e.msg());
    } catch (EmptyArrayException &e) {
        throw SectionBuilderException(e.msg() + " in section [options]");
    } catch (EmptyKeyException &e) {
        throw SectionBuilderException(e.msg() + " in section [options]");
    } catch (ValuesAfterEndingTokenException &e) {
        throw SectionBuilderException(e.msg() + " in section [options]");
    }
}

auto OptionsBuilder::TryParseMultilineValue(const string &line) -> bool {
    if (array_parser_.IsParsing()) {
        array_parser_.ParseLine(line);
        CheckParserHasFinished();
        return true;
    }

    if (array_parser_.StartParsing(line)) {
        CheckParserHasFinished();
        return true;
    }

    return false;
}

void OptionsBuilder::CheckParserHasFinished() {
    if (!array_parser_.IsParsing()) {
        SaveValuesOfParser(array_parser_);
        array_parser_.Reset();
    }
}

void OptionsBuilder::SetAttributeForKey(const string &key,
                                        const string &value) {
    static const auto *const section_err_msg = " in section [options]";
    try {
        TrySetAttributeForKey(key, value);
    } catch (AttributeNotFoundException &e) {
        throw SectionBuilderException(e.msg());
    } catch (BooleanParseException &e) {
        const auto msg = e.msg() + " while parsing key '" + key + "'";
        throw SectionBuilderException(msg + section_err_msg);
    } catch (std::invalid_argument &e) {
        throw SectionBuilderException("Invalid integer found in '" + key +
                                      "' in section [options]");
    }
}
