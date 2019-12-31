module libtt.services.script;

import libtt.services.environment : Environment;

class Script
{
public:
    enum Type
    {
        Bash,
        Custom,
        Execline,
        Sh,
        Zsh
    };

    @property Type type () { return m_type; }
    @property string execute () { return m_execute; }
    @property string shebang () { return m_shebang; }
    @property string user () { return m_user;}
    @property string group () { return m_group; }
    @property ref const(Environment) environment () { return m_env; }

    this (
        string execute,
        string user,
        string group,
        Environment environment,
        Type type,
        string shebang = ""
    ) {
        m_execute = execute;
        m_user = user;
        m_group = group;
        m_env = environment;
        m_type = type;
        setShebang(shebang);
    }

private:
    void setShebang (string shebang)
    {
        switch (type)
        {
            // TODO: handle both shebang and execute script when
            // type != Type.Execline
            default:
                break;
        }
    }

    Type m_type;
    string m_execute;
    string m_shebang;
    string m_user;
    string m_group;
    Environment m_env;
}

