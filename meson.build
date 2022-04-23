project(
    'vhdl-parser',
    'cpp',
    version: '0.0.1',
    license: 'MIT',
    default_options: [
        'cpp_std=c++20'
    ]
)

src = [
    'src/main.cpp',
    'src/error.cpp',
    'src/lexer/lexer_utils.cpp',
    'src/lexer/lexer_defs.cpp',
    'src/parser/parser_defs.cpp',
    'src/parser/vhdl_sem.tab.cc',
]

inc = include_directories(
    'src/',
    'lib/'
)

# Generate re2c pre-processed lexer sources
re2c = find_program('re2c')

gen_lexer = custom_target(
    'gen-lexer',
    input: ['src/lexer/lexer.cpp'],
    output: ['lexer_out.cpp'],
    command: [
        re2c, '@INPUT@', '-o', '@OUTPUT0@', '-i', '--case-ranges'
    ]
)

re2c_lexer_out = declare_dependency(
    sources: [gen_lexer],
    include_directories: ['src/lexer/']
)

executable(
    'Lexer',
    sources: src,
    include_directories: inc,
    dependencies: [
        re2c_lexer_out
    ]
)