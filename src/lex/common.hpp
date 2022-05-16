#pragma once

#include "error.hpp"

#include <cstdint>
#include <optional>
#include <result/result.hpp>

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
        std::optional<std::string> msg = std::nullopt);

    friend std::ostream& operator<<(std::ostream& out, const ParseError& err);
};


/**
 * @brief Parse VHDL based literal.
 * 
 * @param offset    File offset
 * @param view      Raw string_view into backing buffer
 * @return  cpp::result<BasedLiteral, ParseError> 
 */
cpp::result<BasedLiteral, ParseError> parse_based_literal(
    size_t offset,
    std::string_view view) noexcept;
cpp::result<CharacterLiteral, ParseError> parse_character_literal(
    size_t offset,
    std::string_view view) noexcept;
cpp::result<BitStringLiteral, ParseError> parse_bitstring_literal(
    size_t offset,
    std::string_view view) noexcept;
cpp::result<DecimalLiteral, ParseError> parse_decimal_literal(
    size_t offset,
    std::string_view view) noexcept;
BasicIdentifier parse_basic_identifier(
    size_t offset,
    std::string_view view);
ExtendedIdentifier parse_extended_identifier(
    size_t offset,
    std::string_view view) noexcept;
Comment parse_comment(size_t offset, std::string_view view) noexcept;


cpp::result<int32_t, ParseError> parse_int32_t(
    const std::string& str,
    int32_t base = 10) noexcept;

std::ostream& operator<<(std::ostream& out, const parse_error_kind& kind);
} // namespace lexer