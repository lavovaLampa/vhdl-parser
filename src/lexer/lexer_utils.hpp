#ifndef LEXER_UTILS
#define LEXER_UTILS

#include "error.hpp"
#include "result/result.hpp"

#include <cstdint>
#include <iostream>
#include <string>

namespace Lexer {

enum class parse_error_kind {
    invalid_argument,
    out_of_range
};

struct ParseError : Error {
    parse_error_kind kind;

    ParseError(
        parse_error_kind kind,
        std::optional<std::string> msg = std::nullopt
    )
        : kind(kind)
    {
        Error(msg);
    }

    inline std::ostream& operator<<(std::ostream& out)
    {
        out << "Parse Error Kind: "
            << this->kind
            << "\n"
            << Error::operator<<;

        return out;
    }
};

auto parse_int32_t(
    const std::string& str,
    int32_t base = 10
) noexcept -> cpp::result<int32_t, ParseError>;

std::ostream& operator<<(std::ostream& out, parse_error_kind kind);

} // namespace Lexer

#endif /* LEXER_UTILS */
