// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.script;

import std.ascii : newline;

import libtt.data.environment : Environment;

@safe:
nothrow:

class Script
{
public:
    enum Type
    {
        Bash,
        Execline,
    }

    @property Type type() const
    {
        return m_type;
    }

    @property string execute() const
    {
        return m_execute;
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

    this(in Type type, in string execute, Environment environment)
    {
        m_type = type;
        m_execute = execute;
        m_env = environment;
    }

    void prependCode(in string code)
    {
        m_execute = code ~ newline ~ m_execute;
    }

    void appendCode(in string code)
    {
        m_execute ~= code;
    }

protected:
    string m_execute;
private:
    Type m_type;
    string m_user;
    string m_group;
    Environment m_env;
}
