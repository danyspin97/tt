tt_src = files(
    'environment_generator.cpp',
    'signal.cpp',
)

tt_cli_src = []

# Add sources
subdir('data')
subdir('dependency_graph')
subdir('log')
subdir('net')
subdir('parser')
subdir('path')
subdir('request')
subdir('script')
subdir('supervision')
subdir('svc')
subdir('utils')

tt_soversion = 0

libtt = library('tt',
    tt_src,
    include_directories: [ext_inc, tt_inc],
    dependencies: deps,
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
