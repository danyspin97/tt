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

#pragma once

#include <string>

#include "tt/data/service_options.hpp"

namespace tt {
class ServiceVisitor;

class ServiceImpl {
public:
    [[nodiscard]] std::string name() const noexcept { return name_; }

    [[nodiscard]] std::string polish_name() const noexcept {
        return polish_name_;
    }

    [[nodiscard]] std::string description() const noexcept {
        return description_;
    }

    [[nodiscard]] std::string path() const noexcept { return path_; }

    ServiceOptions &options() noexcept { return options_; }

    [[nodiscard]] const ServiceOptions &options() const noexcept {
        return options_;
    }

    virtual std::ostream &Dump(std::ostream &oss) const;

    virtual ~ServiceImpl() = default;
    virtual void Accept(ServiceVisitor &) = 0;

protected:
    ServiceImpl(std::string name, std::string polish_name,
                std::string description, std::string path,
                ServiceOptions &options);

private:
    void ValidatePath() const;

    std::string name_;
    std::string polish_name_;
    std::string description_;
    std::string path_;
    ServiceOptions options_;
};

} // namespace tt
