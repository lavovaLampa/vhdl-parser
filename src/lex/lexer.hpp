#pragma once

#include "token.hpp"

#include <array>
#include <cstdint>
#include <queue>
#include <optional>
#include <string_view>

namespace lexer {

class Lexer {
private:
    const int32_t lookahead;

    std::string_view source;
    const char* cursor;

    std::queue<std::optional<Token>> tokens;

public:
    Lexer(std::string_view input, int32_t lookahead = 1);
    std::optional<Token> pop();
    const std::queue<std::optional<Token>>& peek() const noexcept;
    void reset() noexcept;
};

std::optional<Token> lex(std::string_view src, const char* cursor);

} // namespace lexer
