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
    'parser/bundle_director.d',
    'parser/bundle_options_builder.d',
    'parser/dummy_builder.d',
    'parser/environment_builder.d',
    'parser/instance_service_parser.d',
    'parser/key_value_parser.d',
    'parser/logger_script_builder.d',
    'parser/longrun_director.d',
    'parser/longrun_options_builder.d',
    'parser/main_section_builder.d',
    'parser/main_section.d',
    'parser/multiline_code_parser.d',
    'parser/multiline_value_parser.d',
    'parser/oneshot_director.d',
    'parser/oneshot_options_builder.d',
    'parser/options_builder.d',
    'parser/parser_factory.d',
    'parser/service_parser.d',
    'parser/services_parser.d',
    'parser/script_builder.d',
    'parser/section_builder.d',
    'parser/section_line_parser.d',
    'parser/service_director.d',
    'parser/system_services_parser.d',
    'parser/user_services_parser.d',
    'parser/utils.d',
    'services/bundle.d',
    'services/bundle_options.d',
    'services/environment.d',
    'services/logger_script.d',
    'services/longrun.d',
    'services/longrun_options.d',
    'services/oneshot.d',
    'services/oneshot_options.d',
    'services/script.d',
    'services/service.d',
    'services/service_options.d',
    'signal.d'
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
