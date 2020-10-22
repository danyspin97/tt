catch_cxxflags = [
    '-DCATCH_CONFIG_MAIN',
    '-DCATCH_CONFIG_FAST_COMPILE',
    '-DCATCH_CONFIG_EXPERIMENTAL_REDIRECT',
    '-DCATCH_CONFIG_POSIX_SIGNALS',
]

tt_test_deps = [ tt_dep, deps ]

# When enabling coverage, add a shared library containing all
# tt_cli_src files. This fixes gcov coverage for SonarCloud
if get_option('b_coverage')
    libtt_cli = library('tt_cli',
        tt_cli_src,
        include_directories: [ext_inc, tt_inc],
        dependencies: [ tt_dep, deps ],
        install: false,
        version: meson.project_version(),
        soversion: tt_soversion,
    )

    tt_cli_dep = declare_dependency(
        link_with: libtt_cli,
        include_directories: [ext_inc, tt_inc],
    )

    tt_test_deps += tt_cli_dep
endif

tt_test_src = files(
    'signal.cpp',
)

# Contains an array of array of sources, e.g.:
# [
#   [
#     src1,
#     dep1,
#   ],
#   [
#     src2,
#   ],
# ]
tt_test_multiple_src = []

subdir('dependency_graph')
subdir('parser')
subdir('script')
subdir('svc')
subdir('utils')

# Adapt tt_test_src (which is a flat array) to tt_test_multiple_src format
foreach src: tt_test_src
     tt_test_multiple_src += [[ src ]]
endforeach

# Available from 0.53.0
fs = import('fs')
foreach src : tt_test_multiple_src
    fmt = '@0@'
    # Convert file to string
    filename = fmt.format(src[0])
    # Remove test_ prefix
    # From 0.56.0
    # filename = filename.substring(5)
    # Remove cpp suffix
    filename = fs.replace_suffix(filename, '')
    # Replace path separators ('/') with underscore
    filename = filename.underscorify()

    # When coverage is enabled, libtt_cli will contains all the symbols
    # except for the test file, which is src[0]
    if get_option('b_coverage')
        src = src[0]
    endif

    tt_test_exe = executable(
        filename,
        src,
        dependencies: tt_test_deps,
        cpp_args: catch_cxxflags,
    )

    test(filename, tt_test_exe)
endforeach
