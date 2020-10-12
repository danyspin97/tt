tt_src = files(
    'dirs.cpp',
    'environment_generator.cpp',
    'signal.cpp',
    'status.cpp',
    'user_dirs.cpp',
)

tt_cli_src = []

# Add sources
subdir('action')
subdir('script')
subdir('data')
subdir('dependency_graph')
subdir('parser')
subdir('svc')
subdir('utils')

tt_soversion = 0

libtt = library('tt',
    tt_src,
    include_directories: [ext_inc, tt_inc],
    dependencies: [fmt_dep, spdlog_dep, nng_dep],
    install: true,
    version: meson.project_version(),
    soversion: tt_soversion,
)

tt_dep = declare_dependency(
    link_with: libtt,
    include_directories: [ext_inc, tt_inc],
)

pkg.generate(name: 'tt',
    libraries: libtt,
    subdirs: 'tt',
    version: meson.project_version(),
    description: 'tt library to manage services.',
)

# Add CLI interface
subdir('cli')
