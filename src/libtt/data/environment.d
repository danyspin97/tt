// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.environment;

@safe:
nothrow:

class Environment
{
public:
    const(string) get(string key) const
    {
        return env[key];
    }

    ref const(string[string]) getAll() const
    {
        return env;
    }

    void set(in string key, in string value)
    {
        env[key] = value;
    }

    bool setUnique(in string key, in string value)
    {
        if (key in env)
        {
            return false;
        }

        env[key] = value;
        return true;
    }

    override string toString() const
    {
        import std.format : format;

        string ret;
        foreach (key, value ; env)
        {
            ret ~= format(`\n%s = "%s"`, key, value);
        }
        return ret[1 .. $];
    }

private:
    string[string] env;
}
