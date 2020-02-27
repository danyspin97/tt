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
    'parser/line/key_value_parser.d',
    'parser/line/multiline_code_parser.d',
    'parser/line/multiline_value_parser.d',
    'parser/line/package.d',
    'parser/line/section_line_parser.d',
    'parser/section/bundle_options_builder.d',
    'parser/section/code_section_builder.d',
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
    'parser/word/key_parser.d',
    'parser/word/package.d',
    'parser/word/section_word_parser.d',
    'parser/word/token_parser.d',
    'parser/word/until_token_parser.d',
    'parser/word/value_parser.d',
    'parser/word/whitespace_parser.d',
    'parser/utils.d',
    'data/bundle.d',
    'data/bundle_options.d',
    'data/environment.d',
    'data/logger_script.d',
    'data/longrun.d',
    'data/longrun_options.d',
    'data/oneshot.d',
    'data/oneshot_options.d',
    'data/package.d',
    'data/script.d',
    'data/service.d',
    'data/service_options.d',
    'serialization/longrun_serialization.d',
    'serialization/longrun_options_serialization.d',
    'serialization/oneshot_serialization.d',
    'serialization/oneshot_options_serialization.d',
    'serialization/service_serialization.d',
    'serialization/service_options_serialization.d',
    'serialization/script_serialization.d',
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
