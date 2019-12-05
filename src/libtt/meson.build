libsrc = [
    'service.d'
]

libtt_soversion = 0

libtt = library('libtt',
    libsrc,
    dependencies: [],
    install: true,
    version: meson.project_version(),
    soversion: libtt_soversion
)

pkg.generate(name: 'libtt',
    libraries: libtt,
    subdirs: 'd/libtt',
    version: meson.project_version(),
    description: 'tt library to manage services.',
)

install_subdir('.', install_dir: get_option('includedir') / 'd/libtt/')

