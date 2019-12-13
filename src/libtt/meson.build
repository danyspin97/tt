libsrc = [
    'bundle.d',
    'env.d',
    'longrun.d',
    'oneshot.d',
    'service.d',
    '/usr/include/d/confini.d'
]

libtt_soversion = 0

libtt = library('libtt',
    libsrc,
    dependencies: [ libconfini ],
    install: true,
    version: meson.project_version(),
    soversion: libtt_soversion,
    link_args: ['-shared', '-link-defaultlib-shared'],
)

libtt_dep = declare_dependency(
    link_with: libtt,
    include_directories: tt_inc,
    link_args: '-link-defaultlib-shared',
)

pkg.generate(name: 'libtt',
    libraries: libtt,
    subdirs: 'd/libtt',
    version: meson.project_version(),
    description: 'tt library to manage services.',
)

install_subdir('.', install_dir: get_option('includedir') / 'd/libtt/')

