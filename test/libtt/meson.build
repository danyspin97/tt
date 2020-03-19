libtt_test_src = files(
    'parser/line/key_value_parser.cpp'
)

libtt_test_exe = executable(
    'libtt_test',
    libtt_test_src,
    dependencies: [libtt_dep, catch2_dep],
    cpp_args: '-DCATCH_CONFIG_MAIN',
)

test('Run libtt test suite', libtt_test_exe)

