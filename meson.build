project('tt', 'cpp',
    version: '0.0.1',
    default_options: [
        'cpp_std=c++14',
        'warning_level=3',
        'werror=true',
    ],
)

pkg = import('pkgconfig')

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

systemdir = get_option('systemdir')
if systemdir == 'localstatedir/lib/tt'
    systemdir = localstatedir / 'lib/tt'
endif

servicedir = get_option('servicedir')
if servicedir == 'datadir/tt/service'
    servicedir =  datadir / 'tt/service'
endif

default_log_user = get_option('default_log_user')
default_log_group = get_option('default_log_group')
if default_log_group == 'default_log_user'
    default_log_group = default_log_user
endif

subdir('include')
subdir('src')

if catch2_dep.found()
    subdir('test')
endif

