#ifndef LEXER
#define LEXER

#include "lexer_defs.h"

namespace Lexer {

struct LexerState {
    const char* const base;
    const char* cursor;
    const char* const limit;
};

std::optional<Token> lex(LexerState& state);

} // namespace Lexer

#endif /* LEXER */
