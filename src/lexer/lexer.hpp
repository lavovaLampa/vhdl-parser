#ifndef LEXER
#define LEXER

#include "result/result.hpp"
#include "lexer_defs.hpp"
#include "lexer_utils.hpp"

namespace Lexer {

struct LexerState {
    const char* const base;
    const char* cursor;
    const char* const limit;
};

auto lex(LexerState& state) -> cpp::result<Token, ParseError>;

} // namespace Lexer

#endif /* LEXER */
