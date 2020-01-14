// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.services.longrun_options;

import std.exception : enforce;

import libtt.services.service_options : ServiceOptions;
import libtt.signal : Signal;

class LongrunOptions : ServiceOptions
{
public:
    @property uint notify() { return m_notify; }
    @property void notify(uint notify) { m_notify = notify; }

    @property uint timeoutFinish() { return m_timeoutFinish; }
    @property void timeoutFinish(uint timeoutFinish)
    {
        m_timeoutFinish = timeoutFinish;
    }

    @property uint timeoutKill() { return m_timeoutKill; }
    @property void timeoutKill(uint m_timeoutKill)
    {
        m_timeoutKill = m_timeoutKill;
    }

    @property Signal downSignal() { return m_downSignal; }
    @property void downSignal(Signal downSignal)
    {
        m_downSignal = downSignal;
    }

    @property ushort maxDeath() { return m_maxDeath; }
    @property void maxDeath(ushort maxDeath)
    {
        auto msg = "max_death property cannot be greater than 4096";
        enforce(maxDeath < 4096, msg);
        m_maxDeath = maxDeath;
    }

private:
    uint m_notify;
    uint m_timeoutFinish = 5000;
    uint m_timeoutKill = 0;
    Signal m_downSignal = Signal.SIGTERM;
    ushort m_maxDeath = 3;
}

