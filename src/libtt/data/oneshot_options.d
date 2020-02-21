// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.oneshot_options;

import libtt.data.service_options : ServiceOptions;

class OneshotOptions : ServiceOptions
{
public:
    @property bool optional()
    {
        return m_optional;
    }

    @property void optional(bool optional)
    {
        m_optional = optional;
    }

    @property bool writeMessage()
    {
        return m_writeMessage;
    }

    @property void writeMessage(bool writeMessage)
    {
        m_writeMessage = writeMessage;
    }

private:
    bool m_optional = false;
    bool m_writeMessage = true;
}
