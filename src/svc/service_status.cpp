
#include "tt/svc/service_status.hpp"

#include <cassert> // for assert
#include <map>     // for map

#include "tt/exception.hpp" // for Exception

auto tt::GetServiceStatusName(tt::ServiceStatus status) -> std::string {
    switch (status) {
    case ServiceStatus::Reset:
        return "reset";
    case ServiceStatus::Starting:
        return "starting";
    case ServiceStatus::Stopping:
        return "stopping";
    case ServiceStatus::Up:
        return "up";
    case ServiceStatus::Down:
        return "down";
    default:
        assert(false);
    }
}

auto tt::ParseServiceStatusFromString(const std::string &status)
    -> ServiceStatus {
    static const std::map<std::string, ServiceStatus> signal_strings{
        {"reset", ServiceStatus::Reset},
        {"starting", ServiceStatus::Starting},
        {"stopping", ServiceStatus::Stopping},
        {"up", ServiceStatus::Up},
        {"down", ServiceStatus::Down}};
    if (auto itr = signal_strings.find(status); itr != signal_strings.end()) {
        return itr->second;
    }

    const auto error_msg = "Signal \"" + status + "\" is not valid";
    throw tt::Exception(error_msg);
}
