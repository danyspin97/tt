tt_spawn_script_exe = executable(
    'test_spawn_script_cpp',
    [
        'spawn_script.cpp',
        '../../src/svc/spawn_script.cpp',
        '../../src/svc/utils.cpp',
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
