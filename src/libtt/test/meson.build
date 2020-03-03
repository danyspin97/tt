test_files = [
    'environment_section',
    'invalid',
    'invalid_environment',
    'invalid_int_value',
    'invalid_multiline_value',
    'logger_script',
    'logger_script_no_execute',
    'longrun_options_section',
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

