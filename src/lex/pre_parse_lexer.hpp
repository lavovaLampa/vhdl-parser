#pragma once

#include "lexer.hpp"

#include <string_view>
#include <tl/optional.hpp>

namespace lexer {

tl::optional<TokenVariant> lex_pre_parse(std::string_view src, const char* cursor);

}