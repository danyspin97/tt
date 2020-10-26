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

#ifndef TT_ENVIRONMENT_BUILDER_HPP_
#define TT_ENVIRONMENT_BUILDER_HPP_

#include <string> // for string

#include "tt/parser/section/section_builder.hpp" // for SectionBuilder

namespace tt {
class Environment;

class EnvironmentBuilder : public SectionBuilder {
public:
    explicit EnvironmentBuilder(Environment &environment);

    void ParseLine(const std::string &line) override;

    void EndParsing() override {}

private:
    void TryParseLine(const std::string &line);
    static void CheckKeyIsValid(const std::string &key);

    Environment &environment_;
};

} // namespace tt

#endif // TT_ENVIRONMENT_BUILDER_HPP_
