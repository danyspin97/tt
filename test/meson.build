tt_test_src = files(
    'signal.cpp',
)

subdir('dependency_graph')
subdir('parser')
subdir('svc')

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

