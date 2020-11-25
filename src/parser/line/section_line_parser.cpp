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

#include "tt/parser/line/section_line_parser.hpp"

#include "tt/parser/define.hpp"
#include "tt/utils/trim.hpp"

void tt::SectionLineParser::ParseLine() noexcept {
    valid_ = false;
    utils::trim(line_);
    if (line_[0] != kSectionOpenToken) {
        return;
    }

    auto end_token_pos = line_.find(kSectionCloseToken);
    if (end_token_pos == std::string::npos) {
        return;
    }

    // No characters are allowed after the end of kSectionCloseToken
    if (end_token_pos + 1 != line_.size()) {
        return;
    }

    section_ = line_.substr(1, end_token_pos - 1);
    utils::trim(section_);
    valid_ = true;
}
