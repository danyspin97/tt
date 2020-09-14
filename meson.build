project('tt', 'cpp',
    version: '0.0.1',
    license: 'GPL3',
    meson_version: '>=0.50.0',
    default_options: [
        'cpp_std=c++17',
        'warning_level=3',
        'werror=true',
    ],
)
add_project_arguments('-Wno-missing-braces', language: 'cpp')

pkg = import('pkgconfig')

fmt_dep = dependency('fmt', fallback: ['fmt', 'fmt_dep'])
if fmt_dep.type_name() == 'pkgconfig'
    add_global_arguments('-DSPDLOG_FMT_EXTERNAL', language: 'cpp')
endif

spdlog_dep = dependency('spdlog', fallback: ['spdlog', 'spdlog_dep'])
catch2_dep = dependency('catch2', required: get_option('tests'), fallback: ['catch2', 'catch2_dep'])

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

execline_prefix = get_option('execline_prefix')
if execline_prefix == 'bindir'
    execline_prefix = bindir
endif

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
endif

run_target('coverage-sonarcloud',
    command: ['scripts/coverage-sonarcloud.sh'])

