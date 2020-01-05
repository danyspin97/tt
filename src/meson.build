subdir('libtt')

# Available from 0.53.0
# import('fs')
foreach src : libsrc
    fmt = '@0@'
    file_name = fmt.format(src).underscorify()
    # file_name = fs.replace_suffix(src, '')
    libtt_test_exe = executable('test_' + file_name + '_exe',
        ['libtt/test/main.d', src],
        link_with: libtt,
        link_args: link_runtime_shared_arg,
        d_unittest: true
    )
    test('test_' + file_name, libtt_test_exe)
endforeach

subdir('tt')
