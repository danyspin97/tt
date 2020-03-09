// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.longrun_options;

@safe:
nothrow:

import std.exception : enforce;

import libtt.data.service_options : ServiceOptions;
import libtt.signal : Signal;

class LongrunOptions : ServiceOptions
{
public:
    @property uint notify() const
    {
        return m_notify;
    }

    @property void notify(in uint notify)
    {
        m_notify = notify;
    }

    @property uint timeoutFinish() const
    {
        return m_timeoutFinish;
    }

    @property void timeoutFinish(in uint timeoutFinish)
    {
        m_timeoutFinish = timeoutFinish;
    }

    @property uint timeoutKill() const
    {
        return m_timeoutKill;
    }

    @property void timeoutKill(uint timeoutKill)
    {
        m_timeoutKill = timeoutKill;
    }

    @property Signal downSignal()
    {
        return m_downSignal;
    }

    @property void downSignal(Signal downSignal)
    {
        m_downSignal = downSignal;
    }

    @property ushort maxDeath() const
    {
        return m_maxDeath;
    }

    @property void maxDeath(in ushort maxDeath)
    {
        immutable auto msg = "max_death property cannot be greater than 4096";
        enforce(maxDeath < 4096, msg);
        m_maxDeath = maxDeath;
    }

    @property bool writeMessage() const
    {
        return m_writeMessage;
    }

    @property void writeMessage(in bool writeMessage)
    {
        m_writeMessage = writeMessage;
    }

    @property bool optional() const
    {
        return m_optional;
    }

    @property void optional(in bool optional)
    {
        m_optional = optional;
    }

    override string toString()
    {
        import std.format : format;

        auto ret = super.toString();
        ret = format("\ntimeout_finish = %u\ntimeout_kill = %u", timeoutFinish, timeoutKill);
        ret ~= format("\ndown_signal = %s\nmax_death = %u", downSignal, maxDeath);
        ret ~= format("\nwrite_message = %b\noptional = %b", writeMessage, optional);
        if (notify != 0)
        {
            ret ~= format("\nnotify = %u", notify);
        }
        return ret;
    }

private:
    uint m_notify;
    uint m_timeoutFinish = 5000;
    uint m_timeoutKill = 0;
    Signal m_downSignal = Signal.SIGTERM;
    ushort m_maxDeath = 3;
    bool m_writeMessage = true;
    bool m_optional = false;
}
