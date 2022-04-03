#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <variant>

#include "lexer/lexer.hpp"
#include "lexer/lexer_defs.hpp"
#include "lexer/lexer_utils.hpp"

namespace fs = std::filesystem;

std::optional<std::ifstream> load_file(fs::path file_path);
std::string read_whole_file(std::ifstream& file_stream);

std::optional<std::ifstream> load_file(fs::path file_path)
{
    if (fs::is_regular_file(file_path)) {
        std::ifstream fjel { file_path, std::ios::ate };

        return fjel;
    }
}

std::string read_whole_file(std::ifstream& file_stream)
{
    auto size { file_stream.tellg() };
    std::string file_contents(size, '\0');
    file_stream.seekg(0);

    file_stream.read(file_contents.data(), size);

    return file_contents;
}

int main(int argc, char const* argv[])
{
    if (argc == 2) {
        std::string input_file { argv[1] };

        auto opt_file_stream { load_file(fs::path(input_file)) };
        if (!opt_file_stream.has_value()) {
            std::cerr << "Invalid file specified, exiting...";
            exit(EXIT_FAILURE);
        }

        std::ifstream& file_stream { opt_file_stream.value() };
        std::string file_contents { read_whole_file(file_stream) };

        Lexer::LexerState lexer_state {
            .base { file_contents.data() },
            .cursor { file_contents.data() },
            .limit { file_contents.data() + file_contents.size() }
        };

        while (lexer_state.cursor < lexer_state.limit) {
            auto result { Lexer::lex(lexer_state) };

            if (result.has_value()) {
                std::visit([](auto& arg) { std::cout << arg << std::endl; }, result.value());
            } else {
                auto error { result.error() };
                std::cout << error << std::endl;
            }
        }
    } else {
        std::cerr << "Provide VHDL input file as first argument" << std::endl;
    }

    /* code */
    return 0;
}