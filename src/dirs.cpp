
#include "tt/dirs.hpp"

tt::Dirs &tt::Dirs::GetInstance() {
    static tt::Dirs instance;

    return instance;
}

const std::string &tt::Dirs::bindir() { return bin_; }

const std::string &tt::Dirs::confdir() { return conf_; }

const std::string &tt::Dirs::execline_prefix() { return execline_prefix_; }

const std::string &tt::Dirs::libexecdir() { return libexec_; }

const std::string &tt::Dirs::livedir() { return live_; }

const std::string &tt::Dirs::logdir() { return log_; }

const std::string &tt::Dirs::servicedir() { return service_; }

const std::string &tt::Dirs::systemdir() { return system_; }

const std::string &tt::Dirs::custom_scriptsdir() { return custom_scripts_; }

const std::string &tt::Dirs::admin_servicedir() { return admin_service_; }

const std::string &tt::Dirs::admin_confdir() { return admin_conf_; }
