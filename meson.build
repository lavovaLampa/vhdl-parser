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
    'src/parse/parser_defs.cpp',
    'src/pre_parse/pre_parse.cpp',
    'src/lex/common.cpp',
    'src/lex/lexer.cpp',
    'src/lex/pre_parse_lexer.cpp',
    'src/lex/token.cpp'
]

inc = include_directories(
    'src',
    'lib'
)

# Generate re2c pre-processed lexer sources
re2c = find_program('re2c')

gen_lexer = custom_target(
    'gen-lexer',
    input: ['src/lex/lexer.cpp'],
    output: ['lexer_out.cpp'],
    command: [
        re2c, '@INPUT@', '-o', '@OUTPUT0@', '-i', '--case-ranges'
    ]
)

re2c_lexer_out = declare_dependency(
    sources: [gen_lexer],
    include_directories: ['src/lex/']
)

executable(
    'Lexer',
    sources: src,
    include_directories: inc,
    dependencies: [
        re2c_lexer_out
    ]
)