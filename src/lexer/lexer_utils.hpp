#ifndef LEXER_UTILS
#define LEXER_UTILS

#include "error.hpp"
#include "result/result.hpp"

#include <cstdint>
#include <iostream>
#include <string>

namespace lexer {

enum class parse_error_kind {
    invalid_argument,
    out_of_range
};

struct ParseError : Error {
public:
    parse_error_kind kind;

    ParseError(
        parse_error_kind kind = parse_error_kind::invalid_argument,
        std::optional<std::string> msg = std::nullopt)
        : Error(msg)
        , kind(kind)
    {
    }

    friend std::ostream& operator<<(std::ostream& out, const ParseError& err);
};

auto parse_int32_t(
    const std::string& str,
    int32_t base = 10
) noexcept -> cpp::result<int32_t, ParseError>;
std::ostream& operator<<(std::ostream& out, const parse_error_kind& kind);

} // namespace lexer

#endif /* LEXER_UTILS */
