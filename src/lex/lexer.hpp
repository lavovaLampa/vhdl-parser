#pragma once

#include "token.hpp"

#include <array>
#include <cstdint>
#include <queue>
#include <string_view>
#include <tl/optional.hpp>

namespace lexer {

using LexerFnRef = tl::optional<TokenVariant>(&)(std::string_view, const char* cursor);

class LexerBase {
private:
    LexerFnRef lex_fn;
    const int32_t lookahead;

    std::string_view source;
    const char* cursor;

    std::deque<tl::optional<TokenVariant>> tokens;

protected:
    LexerBase(LexerFnRef lex_fn, int32_t lookahead, std::string_view input);

public:
    tl::optional<TokenVariant> pop();
    const std::deque<tl::optional<TokenVariant>>& peek() const noexcept;
    void reset() noexcept;
};

class PreParseLexer : public LexerBase {
public:
    PreParseLexer(std::string_view input);
};

class Lexer : public LexerBase {
public:
    Lexer(std::string_view input);
};

tl::optional<TokenVariant> lex(std::string_view src, const char* cursor);

} // namespace lexer
