#pragma once

#include "token.hpp"

#include <array>
#include <cstdint>
#include <queue>
#include <string_view>
#include <tl/optional.hpp>

namespace lexer {

class Lexer {
private:
    const int32_t lookahead;

    std::string_view source;
    const char* cursor;

    std::deque<tl::optional<TokenVariant>> tokens;

public:
    Lexer(std::string_view input, int32_t lookahead = 1);
    tl::optional<TokenVariant> pop();
    const std::deque<tl::optional<TokenVariant>>& peek() const noexcept;
    void reset() noexcept;
};

tl::optional<TokenVariant> lex(std::string_view src, const char* cursor);

} // namespace lexer
