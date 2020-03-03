test_files = [
    'environment_section',
    'invalid',
    'invalid_environment',
    'logger_script',
    'logger_script_no_execute',
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

