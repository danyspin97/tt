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

#ifndef LIBTT_SCRIPT_BUILDER_HPP_
#define LIBTT_SCRIPT_BUILDER_HPP_

#include "libtt/data/environment.hpp"
#include "libtt/data/script.hpp"
#include "libtt/parser/section/code_section_builder.hpp"
#include <string>

namespace tt {

class ScriptBuilder : public CodeSectionBuilder {
public:
    ScriptBuilder(Script **script, const Environment &environment,
                  const std::string section)
        : script_(script), environment_(environment), section_(section) {}

    void EndParsing();

protected:
    std::string &GetAttributeForKey(const std::string key);

    std::string &GetCodeAttributeForKey(const std::string key);

    Script::Type GetParsedType();

    Script **script_;
    Environment environment_;

    std::string type_;
    std::string user_;
    std::string group_;
    std::string execute_;
    std::string section_;
};

} // namespace tt

#endif // LIBTT_SCRIPT_BUILDER_HPP_
