dirs_config = configuration_data({
    'version': meson.project_version(),
    'livedir': livedir,
    'systemdir': systemdir,
    'confdir': confdir,
    'logdir': logdir,
    'servicedir': servicedir,
    'bindir': bindir,
    'libexecdir': libexecdir,
    'execline_prefix': execline_prefix
})

configure_file(
    input: 'dirs.d.in',
    output: 'dirs.d',
    configuration: dirs_config,
    install: true,
    install_dir: get_option('includedir') / 'd/libtt'
)

libsrc = [
    'bundle.d',
    'define.d',
    'env.d',
    'longrun.d',
    'oneshot.d',
    'section_parser.d',
    'service.d',
    'service_parser.d',
    meson.current_build_dir() / 'dirs.d',
    '/usr/include/d/confini.d',
]

libtt_soversion = 0

libtt = library('libtt',
    libsrc,
    include_directories: tt_inc,
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

