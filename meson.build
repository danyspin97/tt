project('tt', 'cpp',
    version: '0.0.1',
    license: 'GPL3',
    meson_version: '>=0.50.0',
    default_options: [
        'cpp_std=c++17',
        'warning_level=3',
    ],
)
add_project_arguments('-Werror', language: 'cpp')
add_project_arguments('-Wno-missing-braces', language: 'cpp')

spdlog_cxxargs = [
    # Do not log thread id
    '-DSPDLOG_NO_THREAD_ID',
    # Log levels are set only at startup, no need for atomic levels
    '-DSPDLOG_NO_ATOMIC_LEVELS',
    # Do not let child inherit log file descriptors
    '-DSPDLOG_PREVENT_CHILD_FD',
    # Default logger is created inside CliLogger
    '-DSPDLOG_DISABLE_DEFAULT_LOGGER',
]
add_global_arguments(spdlog_cxxargs, language: 'cpp')


pkg = import('pkgconfig')

fmt_dep = dependency('fmt', fallback: ['fmt', 'fmt_dep'])
spdlog_dep = dependency('spdlog', version : '>=1.4.2', fallback: ['spdlog', 'spdlog_dep'])

# Check that both fmt and spdlog are either installed on the system
# This way spdlog can use the external fmt instead of the embedded one
is_system_fmt = fmt_dep.type_name() == 'pkgconfig'
is_system_spdlog = spdlog_dep.type_name() == 'pkgconfig'
if is_system_fmt and is_system_fmt
    add_global_arguments('-DSPDLOG_FMT_EXTERNAL', language: 'cpp')
endif


catch2_dep = dependency('catch2', required: get_option('tests'), fallback: ['catch2', 'catch2_dep'])
json = dependency('nlohmann_json', fallback: ['nlohmann_json', 'nlohmann_json_dep'])
nng_dep = dependency('nng', method : 'cmake', modules : ['nng::nng'], fallback: ['nng', 'nng_dep'])

tiny_process_inc = include_directories('ext/tiny-process-library')
tiny_process_lib = static_library(
    'tiny-process-library',
    [
        'ext/tiny-process-library/process.cpp',
        'ext/tiny-process-library/process_unix.cpp'
    ],
    include_directories: tiny_process_inc
)
tiny_process_dep = declare_dependency(
    link_with: tiny_process_lib,
    include_directories: tiny_process_inc
)

deps = [
    # It's safe to include catch2 because it will be disabled
    # with the 'tests' option
    catch2_dep,
    fmt_dep,
    json,
    nng_dep,
    spdlog_dep,
    tiny_process_dep,
]

prefix = get_option('prefix')
bindir = get_option('bindir')
datadir = get_option('datadir')
libexecdir = get_option('libexecdir')
localstatedir = get_option('localstatedir')
sysconfdir = get_option('sysconfdir')

if prefix == '/usr/local'
    sysconfdir = prefix / sysconfdir
    localstatedir = prefix / localstatedir
endif

if not bindir.startswith('/')
    bindir = prefix / bindir
endif

if not libexecdir.startswith('/')
    libexecdir = prefix / libexecdir
endif

if not datadir.startswith('/')
    datadir = prefix / datadir
endif

livedir = get_option('livedir')

confdir = get_option('confdir')
if confdir == 'sysconfdir/tt'
   confdir = sysconfdir / 'tt'
endif

logdir = get_option('logdir')
if logdir == 'localstatedir/log/tt'
    logdir = localstatedir / 'log/tt'
endif

statedir = get_option('statedir')
if statedir == 'localstatedir/lib/tt'
    statedir = localstatedir / 'lib/tt'
endif

servicedir = get_option('servicedir')
if servicedir == 'datadir/tt/services'
    servicedir =  datadir / 'tt/services'
endif

default_log_user = get_option('default_log_user')
default_log_group = get_option('default_log_group')
if default_log_group == 'default_log_user'
    default_log_group = default_log_user
endif

subdir('ext')
subdir('include')
subdir('src')

if catch2_dep.found()
    subdir('test')

    if get_option('b_coverage')
        run_target('coverage-sonarcloud',
            command: ['scripts/coverage-sonarcloud.sh'])
    endif
endif
