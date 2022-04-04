project(
    'vhdl-parser',
    'cpp',
    default_options: [
        'cpp_std=c++20'
    ]
)

src = [
    'src/main.cpp',
    'src/error.cpp',
    'src/lexer/lexer_utils.cpp',
    'src/lexer/lexer_defs.cpp'
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