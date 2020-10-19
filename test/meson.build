tt_test_src = files(
    'signal.cpp',
)

subdir('dependency_graph')
subdir('parser')
subdir('script')
subdir('utils')

# Available from 0.53.0
fs = import('fs')
foreach src : tt_test_src
    fmt = '@0@'
    file_name = fmt.format(src).underscorify()
    file_name = fs.replace_suffix(file_name, '')

    tt_test_exe = executable(
        file_name,
        src,
        dependencies: [tt_dep, catch2_dep],
        cpp_args: [
            '-DCATCH_CONFIG_MAIN',
            '-DCATCH_CONFIG_FAST_COMPILE',
            '-DCATCH_CONFIG_EXPERIMENTAL_REDIRECT',
            '-DCATCH_CONFIG_POSIX_SIGNALS',
        ],
    )

    test(file_name, tt_test_exe)
endforeach

libtt_cli = library('tt_cli',
    tt_cli_src,
    include_directories: [ext_inc, tt_inc],
    dependencies: [tt_dep, fmt_dep, spdlog_dep, nng_dep, tiny_process_dep],
    install: false,
    version: meson.project_version(),
    soversion: tt_soversion,
)

tt_cli_dep = declare_dependency(
    link_with: libtt_cli,
    include_directories: [ext_inc, tt_inc],
)

tt_spawn_script_exe = executable(
    'test_spawn_script_cpp',
    'svc/spawn_script.cpp',
    dependencies: [tt_cli_dep, tt_dep, catch2_dep, spdlog_dep, nng_dep, fmt_dep, tiny_process_dep],
    cpp_args: [
        '-DCATCH_CONFIG_MAIN',
        '-DCATCH_CONFIG_FAST_COMPILE',
        '-DCATCH_CONFIG_EXPERIMENTAL_REDIRECT',
        '-DCATCH_CONFIG_POSIX_SIGNALS',
    ],
)

test('test_spawn_script_cpp', tt_spawn_script_exe)
