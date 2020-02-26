test_files = [
    'invalid',
    'main_section',
    'script_section'
]

foreach file : test_files
    configure_file (
        input : file,
        output : file,
        copy: true
    )
endforeach

