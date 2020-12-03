
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
    LiveService() = delete;

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
