
#include "tt/parser/section/section_builder.hpp"

#include "fmt/format.h" // for format

#include "tt/parser/line/key_value_parser.hpp" // for KeyValueParser
#include "tt/parser/parser_error.hpp"          // for ParserError
#include "tt/parser/section/utils.hpp"         // for IsEmptyLine

tt::SectionBuilder::SectionBuilder(std::string section)
    : section_(std::move(section)) {}

auto tt::SectionBuilder::ParseLine(const std::string &line)
    -> tl::expected<void, ParserError> {
    const std::string section_err_msg =
        fmt::format(" in [{}] section", section_);
    if (IsEmptyLine(line)) {
        return {};
    }

    auto ret = ParseMultilineValue(line);
    // The multiline SetArrayAttributeis invalid/has errored
    if (!ret.has_value()) {
        return chain_parser_error<void>(std::move(ret.error()),
                                        section_err_msg);
    }

    // We have found a multiline value and we have also parsed it
    if (ret.value()) {
        return {};
    }

    auto key_value_pair = KeyValueParser::ParseLine(line);
    if (key_value_pair.has_value()) {
        if (auto ret = SetAttribute(std::move(key_value_pair.value().first),
                                    std::move(key_value_pair.value().second));
            !ret.has_value()) {
            return chain_parser_error<void>(std::move(ret.error()),
                                            section_err_msg);
        }
        return {};
    }

    return chain_parser_error<void>(std::move(key_value_pair.error()), "");
}

auto tt::SectionBuilder::EndParsing() -> tl::expected<void, ParserError> {
    if (array_parser_.IsParsing()) {
        return make_parser_error<void>(
            ParserError::Type::ArrayValueNotClosed,
            fmt::format("'{}' has not been closed in [{}] section",
                        array_parser_.key(), section_));
    }

    finished_ = true;
    return {};
}

auto tt::SectionBuilder::ParseMultilineValue(const std::string &line)
    -> tl::expected<bool, ParserError> {
    if (array_parser_.IsParsing()) {
        if (auto ret = array_parser_.ParseLine(line); !ret.has_value()) {
            return chain_parser_error<bool>(std::move(ret.error()), "");
        }

        if (!array_parser_.IsParsing()) {
            if (auto ret = SetArrayAttribute(array_parser_.key(),
                                             array_parser_.values());
                !ret.has_value()) {
                return chain_parser_error<bool>(std::move(ret.error()), "");
            }
            array_parser_.Reset();
        }
        return true;
    }

    auto ret = array_parser_.StartParsing(line);
    if (!ret.has_value()) {
        return chain_parser_error<bool>(std::move(ret.error()), "");
    }
    if (ret.value()) {
        if (!array_parser_.IsParsing()) {
            if (auto ret = SetArrayAttribute(array_parser_.key(),
                                             array_parser_.values());
                !ret.has_value()) {
                return chain_parser_error<bool>(std::move(ret.error()), "");
            }
            array_parser_.Reset();
        }
        return true;
    }

    return false;
}

auto tt::SectionBuilder::SetAttribute(std::string key, std::string value)
    -> tl::expected<void, ParserError> {
    auto valid_keys = GetValidAttributes();
    if (std::find(valid_keys.begin(), valid_keys.end(), key) ==
        valid_keys.end()) {
        return make_parser_error<void>(
            ParserError::Type::InvalidKeyInSection,
            fmt::format("'{}' is not a valid key", key));
    }

    if (values_.find(key) != values_.end()) {
        return make_parser_error<void>(
            ParserError::Type::SectionKeyAlreadySet,
            fmt::format("'{}' has already been set", key));
    }

    values_.emplace(std::move(key), std::move(value));
    return {};
}

auto tt::SectionBuilder::GetAttribute(const std::string &key) const
    -> std::optional<std::string> {
    if (auto value = values_.find(key); value != values_.end()) {
        return value->second;
    }

    return {};
}

auto tt::SectionBuilder::SetArrayAttribute(std::string key,
                                           std::vector<std::string> value)
    -> tl::expected<void, ParserError> {
    auto valid_keys = GetValidArrayAttributes();
    if (std::find(valid_keys.begin(), valid_keys.end(), key) ==
        valid_keys.end()) {
        return make_parser_error<void>(
            ParserError::Type::InvalidKeyInSection,
            fmt::format("'{}' is not a valid key", key));
    }

    if (array_values_.find(key) != array_values_.end()) {
        return make_parser_error<void>(
            ParserError::Type::SectionKeyAlreadySet,
            fmt::format("'{}' has already been set", key));
    }

    array_values_.emplace(std::move(key), value);
    return {};
}

auto tt::SectionBuilder::GetArrayAttribute(const std::string &key) const
    -> std::optional<std::vector<std::string>> {
    if (auto value = array_values_.find(key); value != array_values_.end()) {
        return value->second;
    }

    return {};
}

auto tt::SectionBuilder::SectionParsed() const -> bool { return finished_; }
