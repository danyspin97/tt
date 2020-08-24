tt_spawn_script_exe = executable(
    'test_spawn_script_cpp',
    ['spawn_script.cpp', '../../src/svc/spawn_script.cpp'],
    dependencies: [tt_dep, catch2_dep, spdlog_dep],
    cpp_args: '-DCATCH_CONFIG_MAIN',
)

test('test_spawn_script_cpp', tt_spawn_script_exe)
