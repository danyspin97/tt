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

tt_spawn_script_exe = executable(
    'test_spawn_script_cpp',
    [
        'svc/spawn_script.cpp',
        '../src/svc/spawn_script.cpp',
        '../src/svc/utils.cpp',
    ],
    dependencies: [tt_dep, catch2_dep, spdlog_dep],
    cpp_args: [
        '-DCATCH_CONFIG_MAIN',
        '-DCATCH_CONFIG_FAST_COMPILE',
        '-DCATCH_CONFIG_EXPERIMENTAL_REDIRECT',
        '-DCATCH_CONFIG_POSIX_SIGNALS',
    ],
)

test('test_spawn_script_cpp', tt_spawn_script_exe)
