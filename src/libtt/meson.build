libtt_src = []

# Add sources
subdir('data')
subdir('parser')

libtt_soversion = 0

libtt = library('tt',
    libtt_src,
    include_directories: tt_inc,
    install: true,
    version: meson.project_version(),
    soversion: libtt_soversion,
)

libtt_dep = declare_dependency(
    link_with: libtt,
    include_directories: tt_inc,
)

pkg.generate(name: 'libtt',
    libraries: libtt,
    subdirs: 'libtt',
    version: meson.project_version(),
    description: 'tt library to manage services.',
)

