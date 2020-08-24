tt_cli_src += files(
    'global_options.cpp',
    'log.cpp',
    'main.cpp',
)

subdir('command')
subdir('supervise')

tt_exe = executable(
    'tt',
    tt_cli_src,
    dependencies: [tt_dep, fmt_dep, spdlog_dep]
)

