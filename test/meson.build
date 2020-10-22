tt_test_src = files(
    'signal.cpp',
)

subdir('dependency_graph')
subdir('parser')
subdir('script')
subdir('utils')

catch_cxxflags = [
    '-DCATCH_CONFIG_MAIN',
    '-DCATCH_CONFIG_FAST_COMPILE',
    '-DCATCH_CONFIG_EXPERIMENTAL_REDIRECT',
    '-DCATCH_CONFIG_POSIX_SIGNALS',
]

# Available from 0.53.0
fs = import('fs')
foreach src : tt_test_src
    fmt = '@0@'
    # Convert file to string
    filename = fmt.format(src)
    # Remove cpp suffix
    filename = fs.replace_suffix(filename, '')
    # Replace path separators ('/') with underscore
    filename = filename.underscorify()

    tt_test_exe = executable(
        filename,
        src,
        dependencies: [ tt_dep, deps ],
        cpp_args: catch_cxxflags,
    )

    test(filename, tt_test_exe)
endforeach

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

tt_spawn_script_exe = executable(
    'test_spawn_script_cpp',
    'svc/spawn_script.cpp',
    dependencies: [ tt_cli_dep, tt_dep, deps ],
    cpp_args: catch_cxxflags,
)

test('test_spawn_script_cpp', tt_spawn_script_exe)
