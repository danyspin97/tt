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

#ifndef TT_ENVIRONMENT_BUILDER_HPP_
#define TT_ENVIRONMENT_BUILDER_HPP_

#include "tt/data/environment.hpp"
#include "tt/parser/section/section_builder.hpp"

namespace tt {

class EnvironmentBuilder : public SectionBuilder {
public:
    EnvironmentBuilder(Environment &environment);

    void ParseLine(const std::string &line);

    void EndParsing() {}

private:
    void TryParseLine(const std::string &line);
    static void CheckKeyIsValid(const std::string &key);
    static std::string StripQuotes(const std::string &value);

    Environment &environment_;
};

} // namespace tt

#endif // TT_ENVIRONMENT_BUILDER_HPP_
