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

#pragma once

#include <string>

#include "bitsery/bitsery.h"

namespace tt {
class ServiceVisitor;

class ServiceImpl {
public:
    [[nodiscard]] auto name() const noexcept -> std::string { return name_; }

    [[nodiscard]] auto description() const noexcept -> std::string {
        return description_;
    }

    [[nodiscard]] auto path() const noexcept -> std::string { return path_; }

    virtual auto Dump(std::ostream &oss) const -> std::ostream &;

    virtual ~ServiceImpl() = default;

protected:
    ServiceImpl(std::string &&name, std::string &&description,
                std::string &&path);

    // Required for Bundle(), Longrun() and Oneshot()
    ServiceImpl() = default;

private:
    void ValidatePath() const;

    friend class bitsery::Access;
    template <typename S> void serialize(S &serializer) {
        serializer.template text<sizeof(std::string::value_type), std::string>(
            name_, name_.max_size());
        serializer.template text<sizeof(std::string::value_type), std::string>(
            description_, description_.max_size());
        serializer.template text<sizeof(std::string::value_type), std::string>(
            path_, path_.max_size());
    }

    std::string name_;
    std::string description_;
    std::string path_;
};

} // namespace tt
