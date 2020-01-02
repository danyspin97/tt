project('tt', 'd',
    version: '0.0.1')

pkg = import('pkgconfig')

libconfini = dependency('libconfini')

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

compiler_id = meson.get_compiler('d').get_id()

link_runtime_shared_arg = []

if compiler_id == 'llvm'
    link_runtime_shared_arg = ['-link-defaultlib-shared']
elif compiler_id == 'dmd'
    link_runtime_shared_arg = ['-defaultlib=phobos2', '-debuglib=phobos2']
elif compiler_id == 'gcc'
    link_runtime_shared_arg = ['-shared-libphobos']
endif

tt_inc = include_directories('src')

subdir('src')

