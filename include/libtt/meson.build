define_config = configuration_data({
    'default_log_user': default_log_user,
    'default_log_group': default_log_group
})

define_h = configure_file(
    input: 'define.h',
    output: 'define.h',
    configuration: define_config,
    install: true,
    install_dir: get_option('includedir') / 'libtt'
)

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

dirs_h = configure_file(
    input: 'dirs.h',
    output: 'dirs.h',
    configuration: dirs_config,
    install: true,
    install_dir: get_option('includedir') / 'libtt'
)

libtt_inc = include_directories('.')
