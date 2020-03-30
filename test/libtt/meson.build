libtt_test_src = files(
    'signal.cpp',
)

subdir('parser')

# Available from 0.53.0
#import('fs')
foreach src : libtt_test_src
    fmt = '@0@'
    file_name = fmt.format(src).underscorify()
    #file_name = fs.replace_suffix(src, '')

    libtt_test_exe = executable(
        'test_' + file_name,
        src,
        dependencies: [libtt_dep, catch2_dep],
        cpp_args: '-DCATCH_CONFIG_MAIN',
    )

    test('test_' + file_name, libtt_test_exe)
endforeach

