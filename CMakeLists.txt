cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(Lexer)

add_executable(Lexer
    src/main.cpp
    src/lexer/lexer_defs.cpp
    src/lexer/lexer_out.cpp
)

target_include_directories(Lexer PRIVATE
    src/
)

target_compile_options(Lexer PUBLIC -Wall -Wextra -Wpedantic)