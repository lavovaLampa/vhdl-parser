#pragma once

#include "lexer.hpp"

#include <optional>
#include <string_view>

namespace lexer {

std::optional<Token> lex_pre_parse(std::string_view src, LexerState& state);

}