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

#include "tiny-process-library/process.hpp" // for Process

#include "tt/dependency_graph/service_node.hpp" // for ServiceNode
#include "tt/svc/service_status.hpp"            // for ServiceStatus

namespace tt {

class LiveService {
public:
    explicit LiveService(ServiceNode &&node)
        : node_(std::move_if_noexcept(node)),
          mutex_(std::make_unique<std::mutex>()),
          condition_(std::make_unique<std::condition_variable>()) {}

    [[nodiscard]] auto service() const -> const Service & {
        return node_.service();
    }
    [[nodiscard]] auto name() const -> const std::string & {
        return node_.name();
    }
    [[nodiscard]] auto node() const -> const ServiceNode & { return node_; }

    [[nodiscard]] auto status() const -> ServiceStatus { return status_; }
    void status(ServiceStatus status) { status_ = status; }

    [[nodiscard]] auto supervisor()
        -> std::unique_ptr<TinyProcessLib::Process> && {
        return std::move_if_noexcept(supervisor_);
    }
    void supervisor(std::unique_ptr<TinyProcessLib::Process> process) {
        supervisor_ = std::move_if_noexcept(process);
    }

    void AddUpdatedNode(ServiceNode &&node) { updated_node_ = std::move(node); }
    void MarkForRemoval() { remove_ = true; }
    void UnmarkForRemoval() { remove_ = false; }
    [[nodiscard]] auto IsMarkedForRemoval() const -> bool { return remove_; }

    void Wait() {
        std::unique_lock<std::mutex> lock(*mutex_);
        (condition_)->wait(lock, [this]() { return ended_; });
    }

    void Signal() {
        std::unique_lock<std::mutex> lock(*mutex_);
        ended_ = true;
        condition_->notify_all();
    }

private:
    friend class bitsery::Access;
    LiveService() = default;
    template <typename S> void serialize(S &s) {
        s.object(node_);
        s.ext(updated_node_, bitsery::ext::StdOptional{});
        s.value1b(status_);
        s.object(config_used_);
        s.object(environment_exported_);
        s.value1b(remove_);
    }

    ServiceNode node_;
    std::optional<ServiceNode> updated_node_;
    ServiceStatus status_ = ServiceStatus::Reset;
    Environment config_used_;
    Environment environment_exported_;
    std::unique_ptr<TinyProcessLib::Process> supervisor_;
    bool remove_ = false;

    // Replace this with atomic_flag
    std::unique_ptr<std::mutex> mutex_;
    std::unique_ptr<std::condition_variable> condition_;
    bool ended_ = false;
};

} // namespace tt
