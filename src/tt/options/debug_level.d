// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

module tt.options.debug_level;

/// The debug level to run the `tt` binary in
enum DebugLevel
{
    Error,
    Warn,
    Info,
    Debug,
    Trace,
}
