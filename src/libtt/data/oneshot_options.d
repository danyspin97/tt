// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.oneshot_options;

@safe:
nothrow:

import libtt.data.service_options : ServiceOptions;

class OneshotOptions : ServiceOptions
{
public:
    @property bool optional() const
    {
        return m_optional;
    }

    @property void optional(in bool optional)
    {
        m_optional = optional;
    }

    @property bool writeMessage() const
    {
        return m_writeMessage;
    }

    @property void writeMessage(in bool writeMessage)
    {
        m_writeMessage = writeMessage;
    }

    override string toString()
    {
        import std.format : format;

        auto ret = super.toString();
        if (ret != "")
        {
            ret ~= "\n";
        }
        ret ~= format("write_message = %b\noptional = %b", writeMessage, optional);
        return ret;
    }

private:
    bool m_optional = false;
    bool m_writeMessage = true;
}
