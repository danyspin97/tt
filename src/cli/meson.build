tt_cli_src += files(
    'global_options.cpp',
    'main.cpp',
)

subdir('command')

tt_exe = executable(
    'tt',
    tt_cli_src,
    dependencies: [ tt_dep, deps ],
)

