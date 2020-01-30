// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.services.script;

import libtt.services.environment : Environment;

class Script
{
public:
    @property string execute() { return shebang ~ m_execute; }
    @property void execute(string execute) { m_execute = execute; }
    @property string shebang() { return m_shebang; }
    @property string user() { return m_user;}
    @property void user(string user) { m_user = user; }
    @property string group() { return m_group; }
    @property void group(string group) { m_group = group; }
    @property ref const(Environment) environment () { return m_env; }

    this (
        string execute,
        string shebang,
        Environment environment,
    ) {
        m_execute = execute;
        m_shebang = shebang;
        m_env = environment;
    }

protected:
    string m_execute;
private:
    string m_shebang;
    string m_user;
    string m_group;
    Environment m_env;
}

