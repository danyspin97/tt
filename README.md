# tt

![GitHub](https://img.shields.io/github/license/danyspin97/tt?color=blue&logo=github&style=flat-square)
![Liberapay receiving](https://img.shields.io/liberapay/receives/danyspin97?logo=liberapay&style=flat-square)
[![Build Status](https://img.shields.io/endpoint.svg?url=https%3A%2F%2Factions-badge.atrox.dev%2Fdanyspin97%2Ftt%2Fbadge&style=flat-square)](https://actions-badge.atrox.dev/danyspin97/tt/goto)
[![builds.sr.ht status](https://builds.sr.ht/~danyspin97/tt.svg)](https://builds.sr.ht/~danyspin97/tt?)
[![Travis (.org)](https://img.shields.io/travis/danyspin97/tt?logo=travis&style=flat-square)](https://travis-ci.org/DanySpin97/tt)
[![Coveralls github](https://img.shields.io/coveralls/github/DanySpin97/tt?logo=coveralls&style=flat-square)](https://coveralls.io/github/DanySpin97/tt)
[![CodeFactor](https://img.shields.io/codefactor/grade/github/danyspin97/tt?logo=CodeFactor&style=flat-square)](https://www.codefactor.io/repository/github/danyspin97/tt)
[![Codacy Badge](https://img.shields.io/codacy/grade/964fc2e2d7374cda979ac4b470e93561?logo=Codacy&style=flat-square)](https://www.codacy.com/manual/danyspin97/tt?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DanySpin97/tt&amp;utm_campaign=Badge_Grade)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/DanySpin97/tt.svg?logo=lgtm&logoWidth=18&style=flat-square)](https://lgtm.com/projects/g/DanySpin97/tt/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/DanySpin97/tt.svg?logo=lgtm&logoWidth=18&style=flat-square)](https://lgtm.com/projects/g/DanySpin97/tt/context:cpp)
[![Sonar Quality Gate](https://img.shields.io/sonar/quality_gate/DanySpin97_tt?logo=sonarcloud&server=https%3A%2F%2Fsonarcloud.io&style=flat-square)](https://sonarcloud.io/dashboard?id=DanySpin97_tt)
[![Sonar Violations (short format)](https://img.shields.io/sonar/violations/DanySpin97_tt?logo=sonarcloud&server=https%3A%2F%2Fsonarcloud.io&sonarVersion=7.9&style=flat-square)](https://sonarcloud.io/dashboard?id=DanySpin97_tt)
[![Sonar Tech Debt](https://img.shields.io/sonar/tech_debt/DanySpin97_tt?logo=sonarcloud&server=https%3A%2F%2Fsonarcloud.io&style=flat-square)](https://sonarcloud.io/dashboard?id=DanySpin97_tt)
[![CII Best Practices](https://img.shields.io/cii/summary/3935?style=flat-square)](https://bestpractices.coreinfrastructure.org/projects/3935)

tt is an init and service manager inspired by
[66](https://web.obarun.org/software/66),
[s6](https://skarnet.org/software/s6/) suite and [deamontools](http://cr.yp.to/daemontools.html).

tt offers a valid alternative to systemd for PID 1 and service
management. It uses the supervision to manage long running programs (deamons),
log everything to files (no binary log interface) and provides an easy to use
command line interface.

The services must be compiled before being able to run; this is the biggest difference with the most used service managers (_systemd_, _OpenRC_, _runit_),
which instead are based on runtime services.

## Planned features

- Support for different types of services: _oneshot_, _deamons_ and _bundles_
- Predictable dependencies at build time
- Configurable parameters for services (i.e. change command line arguments
  without editing the service file)
- Asynchrounus start of the services (no [run
  levels](https://en.wikipedia.org/wiki/Runlevel))
- Log everything into files, no syslog needed
- Low footprint
- Target desktop and servers
- Conditional dependencies for complex services (such as web apps)
- Provides sane defaults

## Getting started

### Dependencies

tt supports the following compilers:

- gcc >= 9.0 (gcc-8 does not have a good support for `<filesystem>`, despite including it)
- clang >= 8.0

tt depends on the following libraries:

- [boost](https://www.boost.org/) (build time only)
- [catch2](https://github.com/catchorg/Catch2)* (only required for the tests)
- [fmt](https://github.com/fmtlib/fmt)*
- [json](https://github.com/nlohmann/json)*
- [nng](https://github.com/nanomsg/nng)*
- [spdlog](https://github.com/gabime/spdlog)*

\* _if these libraries are not found in the system, meson will download and use
a local copy_.

### Building

To build tt run the following commands:

```bash
$ meson build
$ ninja -C build
```

## Testing

To run the tt test suite, execute the following commands:

```
$ meson -Dtests=enabled build
$ ninja -C build test
```

## Contributing

Feel free to contribute by opening a Pull Request! Have a look at Github Issues
for the current development tasks.

### Formatting your changes

tt follows the [Google C++ Style
Guide](https://google.github.io/styleguide/cppguide.html) and uses
`clang-format` to apply this style. If you have `clang-format` installed, run
the following command to format your changes:

### Continuous Integration

tt is tested under 3 different CI platforms: 

- Github Actions, testing a meson [unity
  build](https://mesonbuild.com/Unity-builds.html) using a compiler matrix of
  different clang and gcc versions. `address` and `undefined` sanitizers are
  enabled.
- Travis CI, sending coverage to [Coveralls](https://coveralls.io) and analyzing
  the build with [SonarCloud](https://sonarcloud.io).
- [build.sr.ht](https://builds.sr.ht), testing tt on various systems and
  architectures.

All Pull Requests must pass the CI and the static analyzers' checks before
being accepted.

```
$ ninja -C build clang-format
```

## Community

- #tt IRC channel on freenode.

## Develoment updates

tt development updates are posted on @danyspin97 [blog](https://danyspin97.org/tags/tt/).

## Credits

tt is being developed by [@danyspin97](https://github.com/danyspin97) and [@Cogitri](https://github.com/Cogitri).

Thanks to [@pac85](https://github.com/pac85) for its contributions and to all the people that shared ideas for tt.

Credits also to:

- [**@skarnet**](https://github.com/skarnet) for his work on
  [_s6_](https://skarnet.org/software/s6/) and his awesome explanations about
  UNIX inner workings and design
- [**@obarun**](https://obarun.org) for his work on 66 and most of the ideas
  that made the foundation of tt

### Third-party libraries included

- [**args**](https://github.com/Taywee/args), command-line argument parser
- [**bitsery**](https://github.com/fraillt/bitsery), binary serialization
  library
- [**nngpp**](https://github.com/cwzx/nngpp), wrapper around
  [_nanomsg-next-gen_](https://github.com/nanomsg/nng) library
- [**tiny-process-library**](https://gitlab.com/eidheim/tiny-process-library/),
  to start and stop processess and read from stdin
  and stdout
- [**xdgpp**](https://sr.ht/~danyspin97/xdgpp/), implementation of the
  [_XDG Base Directory Specification_](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html)

## License

tt is licensed under the [GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.en.html) License - see [LICENSE](https://github.com/DanySpin97/tt/blob/master/LICENSE) file for more
details.

