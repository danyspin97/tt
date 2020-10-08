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

#ifndef TT_SCRIPT_BUILDER_HPP_
#define TT_SCRIPT_BUILDER_HPP_

#include <string>

#include "tt/data/script.hpp"
#include "tt/parser/section/code_section_builder.hpp"

namespace tt {

class ScriptBuilder : public CodeSectionBuilder {
public:
    explicit ScriptBuilder(const std::string &section);
    auto script() -> Script;
    auto HasScript() const -> bool;
    void EndParsing() override;

protected:
    auto GetAttributeForKey(const std::string &key) -> std::string & override;
    auto GetCodeAttributeForKey(const std::string &key)
        -> std::string & override;
    auto GetParsedType() const -> Script::Type;
    void SetOptionalAttributeForScript(Script &script);

    auto execute() -> const std::string &;
    void execute(std::string &&);
    auto execute_move() -> std::string &&;
    auto type() -> const std::string &;
    void type(const std::string &type);
    auto user() -> const std::string &;
    void user(const std::string &);
    auto user_move() -> std::string &&;
    auto group() -> const std::string &;
    void group(const std::string &);
    auto group_move() -> std::string &&;

private:
    std::string type_;
    std::string user_;
    std::string group_;
    std::string execute_;
    std::string down_signal_;
    std::string max_death_;
    std::string timeout_;
    std::string timeout_kill_;
    bool finished_ = false;
};

} // namespace tt

#endif // TT_SCRIPT_BUILDER_HPP_
