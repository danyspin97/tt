test_files = [
    'environment_section',
    'invalid',
    'invalid_environment',
    'invalid_int_value',
    'invalid_multiline_value',
    'invalid_quotes',
    'logger_script',
    'logger_script_no_execute',
    'longrun_options_section',
    'main_section',
    'script_section',
    'unclosed_quotes',
    'unknown_key',
    'unknown_multiline_value',
]

foreach file : test_files
    configure_file (
        input : file,
        output : file,
        copy: true
    )
endforeach

