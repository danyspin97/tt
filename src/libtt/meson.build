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

dirs_d = configure_file(
    input: 'dirs.d',
    output: 'dirs.d',
    configuration: dirs_config,
    install: true,
    install_dir: get_option('includedir') / 'd/libtt'
)

libsrc = files(
    'parser/key_value_parser.d',
    'parser/main_section.d',
    'parser/section_parser.d',
    'parser/service_parser.d',
    'services/bundle.d',
    'services/environment.d',
    'services/longrun.d',
    'services/oneshot.d',
    'services/service_options.d',
    'services/script.d',
    'services/service.d',
) + [dirs_d]

libtt_soversion = 0

libtt = library('tt',
    libsrc,
    include_directories: tt_inc,
    install: true,
    version: meson.project_version(),
    soversion: libtt_soversion,
    link_args: ['-shared', link_runtime_shared_arg],
)

libtt_dep = declare_dependency(
    link_with: libtt,
    include_directories: tt_inc,
    link_args: link_runtime_shared_arg,
)

pkg.generate(name: 'libtt',
    libraries: libtt,
    subdirs: 'd/libtt',
    version: meson.project_version(),
    description: 'tt library to manage services.',
)

install_subdir(
    '.',
    install_dir: get_option('includedir') / 'd/libtt/',
    exclude_directories: 'test'
)

subdir('test')
