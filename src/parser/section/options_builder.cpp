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

#include <stdexcept> // for invalid_argument

#include "tt/parser/line/exception.hpp"        // for EmptyArrayException
#include "tt/parser/line/key_value_parser.hpp" // for KeyValueParser
#include "tt/parser/section/exception.hpp"     // for SectionBuilderException
#include "tt/parser/section/utils.hpp"         // for IsEmptyLine
#include "tt/utils/parse_boolean.hpp"          // for BooleanParseException

void tt::OptionsBuilder::ParseLine(const std::string &line) {
    if (IsEmptyLine(line)) {
        return;
    }

    if (ParseMultilineValue(line)) {
        return;
    }

    auto key_value_parser = KeyValueParser(line);
    if (key_value_parser.IsLineValid()) {
        const std::string key = key_value_parser.key();
        const std::string value = key_value_parser.value();
        SetAttributeForKey(key, value);
        return;
    }

    throw SectionBuilderException(line + "\n is not valid");
}

void tt::OptionsBuilder::EndParsing() {
    if (array_parser_.IsParsing()) {
        throw SectionBuilderException(
            array_parser_.key() + " has not been closed in section [options]");
    }
}

auto tt::OptionsBuilder::ParseMultilineValue(const std::string &line) -> bool {
    try {
        return TryParseMultilineValue(line);
    } catch (const AttributeNotFoundException &e) {
        throw SectionBuilderException(e.msg());
    } catch (const EmptyArrayException &e) {
        throw SectionBuilderException(e.msg() + " in section [options]");
    } catch (const EmptyKeyException &e) {
        throw SectionBuilderException(e.msg() + " in section [options]");
    } catch (const ValuesAfterEndingTokenException &e) {
        throw SectionBuilderException(e.msg() + " in section [options]");
    }
}

auto tt::OptionsBuilder::TryParseMultilineValue(const std::string &line)
    -> bool {
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

void tt::OptionsBuilder::CheckParserHasFinished() {
    if (!array_parser_.IsParsing()) {
        SaveValuesOfParser(array_parser_);
        array_parser_.Reset();
    }
}

void tt::OptionsBuilder::SetAttributeForKey(const std::string &key,
                                            const std::string &value) {
    static const auto *const section_err_msg = " in section [options]";
    try {
        TrySetAttributeForKey(key, value);
    } catch (const AttributeNotFoundException &e) {
        throw SectionBuilderException(e.msg());
    } catch (const tt::utils::BooleanParseException &e) {
        const auto msg = e.msg() + " while parsing key '" + key + "'";
        throw SectionBuilderException(msg + section_err_msg);
    } catch (const std::invalid_argument & /*e*/) {
        throw SectionBuilderException("Invalid integer found in '" + key +
                                      "' in section [options]");
    }
}
