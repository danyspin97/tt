tt_src = [
    'dirs.cpp',
    'signal.cpp'
]

# Add sources
subdir('data')
subdir('dependency_graph')
subdir('parser')
subdir('supervisor')

tt_soversion = 0

libtt = library('tt',
    tt_src,
    include_directories: [ext_inc, tt_inc],
    dependencies: [fmt_dep, spdlog_dep],
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
