// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.script;

import libtt.data.environment : Environment;

@safe:
nothrow:

class Script
{
public:
    @property string execute() const
    {
        return shebang ~ m_execute;
    }

    @property string shebang() const
    {
        return m_shebang;
    }

    @property string user() const
    {
        return m_user;
    }

    @property void user(in string user)
    {
        m_user = user;
    }

    @property string group() const
    {
        return m_group;
    }

    @property void group(in string group)
    {
        m_group = group;
    }

    @property ref const(Environment) environment() const
    {
        return m_env;
    }

    this(in string execute, in string shebang, Environment environment)
    {
        m_execute = execute;
        m_shebang = shebang;
        m_env = environment;
    }

    void prependCode(in string code)
    {
        m_execute = code ~ m_execute;
    }

    void appendCode(in string code)
    {
        m_execute ~= code;
    }

protected:
    string m_execute;
private:
    string m_shebang;
    string m_user;
    string m_group;
    Environment m_env;
}
