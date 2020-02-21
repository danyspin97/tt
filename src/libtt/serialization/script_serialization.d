module libtt.serialization.script_serialization;

import libtt.data;

import std.conv : to;
import std.json;

class ScriptSerialization
{
public:
    this(Script script)
    {
        m_script = script;
    }

    @property JSONValue json()
    {
        JSONValue j = ["execute" : m_script.execute];
        j.object["shebang"] = m_script.shebang;
        j.object["user"] = m_script.user;

        return j;
    }

    @property Script object()
    {
        return m_script;
    }

private:
    Script m_script;
}
