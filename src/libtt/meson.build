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
    'exception.d',
    'parser/key_value_parser.d',
    'parser/multiline_code_parser.d',
    'parser/multiline_value_parser.d',
    'parser/section_line_parser.d',
    'parser/section/bundle_options_builder.d',
    'parser/section/dummy_builder.d',
    'parser/section/environment_builder.d',
    'parser/section/logger_script_builder.d',
    'parser/section/longrun_options_builder.d',
    'parser/section/main_section_builder.d',
    'parser/section/main_section.d',
    'parser/section/oneshot_options_builder.d',
    'parser/section/options_builder.d',
    'parser/section/package.d',
    'parser/section/script_builder.d',
    'parser/section/section_builder.d',
    'parser/service/bundle_director.d',
    'parser/service/dependency_reader.d',
    'parser/service/instance_service_parser.d',
    'parser/service/longrun_director.d',
    'parser/service/oneshot_director.d',
    'parser/service/package.d',
    'parser/service/parser_factory.d',
    'parser/service/service_director.d',
    'parser/service/service_director_test.d',
    'parser/service/service_parser.d',
    'parser/service/services_parser.d',
    'parser/service/system_services_parser.d',
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
