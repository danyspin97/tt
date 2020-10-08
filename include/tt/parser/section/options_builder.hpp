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

#ifndef TT_OPTIONS_BUILDER_HPP_
#define TT_OPTIONS_BUILDER_HPP_

#include "tt/parser/line/array_parser.hpp"
#include "tt/parser/section/section_builder.hpp"

namespace tt {

class OptionsBuilder : public SectionBuilder {
public:
    void ParseLine(const std::string &line) override;
    void EndParsing() override;

protected:
    auto ParseMultilineValue(const std::string &line) -> bool;

private:
    auto TryParseMultilineValue(const std::string &line) -> bool;

    void CheckParserHasFinished();

    void SetAttributeForKey(const std::string &key, const std::string &value);

protected:
    virtual void SaveValuesOfParser(const ArrayParser &parser) = 0;
    virtual void TrySetAttributeForKey(const std::string &key,
                                       const std::string &value) = 0;

private:
    ArrayParser array_parser_;
};

} // namespace tt

#endif // TT_OPTIONS_BUILDER_HPP_
