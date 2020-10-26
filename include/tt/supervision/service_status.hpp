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

#include <condition_variable> // for condition_variable
#include <mutex>              // for mutex

namespace tt {

class ServiceStatus {
public:
    ServiceStatus() = default;
    // std::mutex and std::condition_variable does not have this constructor
    ServiceStatus(const tt::ServiceStatus &) = delete;

    [[nodiscard]] auto Wait() -> bool;
    void Update(bool succeeded);

private:
    // TODO c++20: use atomic_flag::wait
    std::mutex mutex_;
    std::condition_variable condition_;
    bool success_ = false;
    bool ended_ = false;
};

} // namespace tt
