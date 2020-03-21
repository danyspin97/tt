// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#include "libtt/parser/line/code_parser.hpp"

#include <string>

#include "libtt/parser/utils.hpp"

    #include <iostream>
using std::string;
using tt::CodeParser;

bool CodeParser::StartParsing(const string line) {
    if (IsParsing()) {
        throw new std::exception();
    }
    auto trimmed_line = trim_copy(line);
    auto equal_token_pos = trimmed_line.find('=');
    if (equal_token_pos == string::npos) {
        return false;
    }

    if (equal_token_pos + 1 == trimmed_line.size() ||
        trimmed_line[equal_token_pos + 1] != '(') {
        return false;
    }

    // Are there other chars after '('?
    if (equal_token_pos + 2 != trimmed_line.size())
    {
        return false;
    }

    key_ = trimmed_line.substr(0, equal_token_pos - 1);
    rtrim(key_);
    return is_parsing_ = true;
}

void CodeParser::ParseLine(const string line) {
    auto trimmed_line = trim_copy(line);
    if (trimmed_line == ")") {
        is_parsing_ = false;
        return;
    }

    code_ << line;
    code_ << "\n";
}
