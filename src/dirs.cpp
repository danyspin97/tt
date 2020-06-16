
#include "tt/dirs.hpp"

tt::Dirs &tt::Dirs::GetInstance() {
    static tt::Dirs instance;

    return instance;
}

const std::string &tt::Dirs::bindir() { return bindir_; }

const std::string &tt::Dirs::confdir() { return confdir_; }

const std::string &tt::Dirs::execline_prefix() { return execline_prefix_; }

const std::string &tt::Dirs::libexecdir() { return libexecdir_; }

const std::string &tt::Dirs::livedir() { return livedir_; }

const std::string &tt::Dirs::logdir() { return logdir_; }

const std::string &tt::Dirs::servicedir() { return servicedir_; }

const std::string &tt::Dirs::statedir() { return statedir_; }
