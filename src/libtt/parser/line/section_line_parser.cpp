// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#include "libtt/parser/line/section_line_parser.hpp"

#include "libtt/parser/utils.hpp"

using std::string;
using tt::SectionLineParser;

void SectionLineParser::ParseLine()
{
    valid_ = false;
    trim(line_);
    if (line_[0] != '[')
    {
        return;
    }

    auto end_token_pos = line_.find(']');
    if (end_token_pos == string::npos)
    {
        return;
    }

    // No characters are allowed after the end of ']' token
    if (end_token_pos + 1 != line_.size())
    {
        return;
    }

    section_ = line_.substr(1, end_token_pos - 1);
    trim(section_);
    valid_ = true;
}

