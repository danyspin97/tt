# tt

[![codecov](https://codecov.io/gh/DanySpin97/tt/branch/master/graph/badge.svg)](https://codecov.io/gh/DanySpin97/tt)
[![CodeFactor](https://www.codefactor.io/repository/github/danyspin97/tt/badge)](https://www.codefactor.io/repository/github/danyspin97/tt)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/964fc2e2d7374cda979ac4b470e93561)](https://www.codacy.com/manual/danyspin97/tt?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DanySpin97/tt&amp;utm_campaign=Badge_Grade)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/DanySpin97/tt.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/DanySpin97/tt/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/DanySpin97/tt.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/DanySpin97/tt/context:cpp)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/3935/badge)](https://bestpractices.coreinfrastructure.org/projects/3935)

tt is a Work in Progress init and service manager inspired by [66](https://web.obarun.org/software/66) and based on
the [s6](https://skarnet.org/software/s6/) suite.

tt tries to offer a valid alternative to systemd for PID 1 and service
management. It uses the supervision to manage long running programs (deamons),
log everything to files (no binary log interface) and provides an easy to use
command line interface.

Inheriting [s6-rc](https://skarnet.org/software/s6-rc/) feature, the services must be compiled before being able to
run. This is one of the biggest differences with the current service managers,
which instead are based on runtime services.

tt is designed to run on both desktops and servers, and does not target embedded
devices; if memory and space are constrained (e.g. you're configuring a router),
then the use of s6 configured ad-hoc for that machine is recommended.

## Planned features

- Support for different types of services: _oneshot_, _deamons_ and _bundles_
- Predictable dependencies at build time
- Configurable parameters for services (i.e. change CMDARGS without eding the service file)
- Asynchrounus start of the services (no [run levels](https://en.wikipedia.org/wiki/Runlevel))
- Log everything into files, no syslog needed for deamons
- Low footprint
- Target desktop and servers
- Conditional dependencies for complex services (such as web apps)
- Provides sane defaults
- Prioritize usability

## Getting started

### Dependencies

tt depends on the following libraries:

- [s6](https://skarnet.org/software/s6/)
- [s6-rc](https://skarnet.org/software/s6-rc/)
- [execline](https://skarnet.org/software/execline/)

You can install them using your package manager.

### Building

To build tt a working C++ compiler is required. Meson will take care of the rest:

```bash
$ meson build
$ ninja -C build
```

## Testing

tt test suite uses catch2 header-only library. If this dependency is not found
on the system, meson will download a copy to use in the local repository.
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

tt uses Github Actions for CI, running and checking the test suite; in particular, the CI runs a meson [unity build](https://mesonbuild.com/Unity-builds.html) with `-Werror` enabled and `address`, `undefined` sanitizers enabled. All Pull Requests must pass the CI before being included.

```
$ ninja -C build clang-format
```

## Community

- #tt IRC channel on freenode.

## Develoment updates

tt development updates are posted on @danyspin97 [blog](https://danyspin97.org/tags/tt/).

## Credits

tt is being developed by @danyspin97 and @Cogitri.

Thanks to @pac85 for its contributions and to all the people that shared ideas for tt.

Credits also to:

- @Taywee for its [args](https://github.com/Taywee/args) library
- @skarnet for its s6 supervision suite
- [obarun](https://obarun.org) for his work on 66

## License

tt is licensed under the [GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.en.html) License - see [LICENSE](https://github.com/DanySpin97/tt/blob/master/LICENSE) file for more
details.

