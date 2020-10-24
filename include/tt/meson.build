define_config = configuration_data({
    'default_log_user': default_log_user,
    'default_log_group': default_log_group,
    'version': meson.project_version(),
})

define_h = configure_file(
    input: 'define.h.in',
    output: 'define.h',
    configuration: define_config,
    install: true,
    install_dir: get_option('includedir') / 'tt'
)

dirs_config = configuration_data({
    'livedir': livedir,
    'statedir': statedir,
    'confdir': confdir,
    'logdir': logdir,
    'servicedir': servicedir,
    'bindir': bindir,
    'libexecdir': libexecdir,
    'execline_prefix': execline_prefix
})

subdir('path')
