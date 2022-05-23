#include "token.hpp"

#include <cstdint>
#include <cuchar>

#include <nameof/nameof.hpp>

namespace lexer {

Token::Token(size_t offset, size_t len, token_kind kind)
    : offset { offset }
    , len { len }
    , kind { kind }
{
}

Comment::Comment(size_t offset, size_t len, std::string val)
    : Token { offset, len, token_kind::comment }
    , val { val }
{
}

BitStringLiteral::BitStringLiteral(size_t offset, size_t len, bit_string_base base, std::string val)
    : Token { offset, len, token_kind::bitstring_literal }
    , base { base }
    , val { val }
{
}

CharacterLiteral::CharacterLiteral(size_t offset, size_t len, char8_t val)
    : Token { offset, len, token_kind::character_literal }
    , val { val }
{
}

StringLiteral::StringLiteral(
    size_t offset,
    size_t len,
    std::string val,
    std::optional<op_symbol> operator_symbol
)
    : Token { offset, len, token_kind::string_literal }
    , val { val }
    , operator_symbol { operator_symbol }
{
}

Identifier::Identifier(
    size_t offset,
    size_t len,
    identifier_kind kind,
    std::string val
)
    : Token { offset, len, token_kind::identifier }
    , kind { kind }
    , val { val }
{
}

ReservedWord::ReservedWord(size_t offset, size_t len, reserved_word_kind kind)
    : Token { offset, len, token_kind::reserved_word }
    , kind { kind }
{
}

BasedLiteral::BasedLiteral(
    size_t offset,
    size_t len,
    int32_t base,
    int32_t decimal_part,
    std::optional<int32_t> fraction_part,
    std::optional<int32_t> exponent
)
    : Token { offset, len, token_kind::based_literal }
    , base { base }
    , decimal_part { decimal_part }
    , fraction_part { fraction_part }
    , exponent { exponent }
{
}

DecimalLiteral::DecimalLiteral(
    size_t offset,
    size_t len,
    int32_t decimal_part,
    std::optional<int32_t> fraction_part = std::nullopt,
    std::optional<int32_t> exponent = std::nullopt
)
    : Token { offset, len, token_kind::decimal_literal }
    , decimal_part { decimal_part }
    , fraction_part { fraction_part }
    , exponent { exponent }
{
}

Delimiter::Delimiter(size_t offset, size_t len, delimiter_kind kind)
    : Token { offset, len, token_kind::delimiter }
    , kind { kind }
{
}

std::ostream& operator<<(std::ostream& out, const Token& lex)
{
    // TODO: Improve stringification
    out << lex.offset;

    return out;
}

std::ostream& operator<<(std::ostream& out, const Comment& comment)
{
    out << "Comment("
        << static_cast<const Token&>(comment)
        << ", \""
        << comment.val
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const BitStringLiteral& lit)
{
    out << "BitStringLiteral("
        << static_cast<const Token&>(lit)
        << ", "
        << NAMEOF_ENUM(lit.base)
        << ", \""
        << lit.val
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const CharacterLiteral& lit)
{
    out << "CharacterLiteral("
        << static_cast<const Token&>(lit)
        << ", '"
        << lit.val
        << "')";

    return out;
}

std::ostream& operator<<(std::ostream& out, const StringLiteral& lit)
{
    out << "StringLiteral("
        << static_cast<const Token&>(lit)
        << ", \""
        << lit.val
        << "\"";

    if (lit.operator_symbol.has_value()) {
        out << ", "
            << NAMEOF_ENUM(lit.operator_symbol.value());
    }

    out << ")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const Identifier& id)
{
    out << "Identifier("
        << static_cast<const Token&>(id)
        << ", Kind: "
        << NAMEOF_ENUM(id.kind)
        << ", \""
        << id.val
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const ReservedWord& id)
{
    out << "ReservedWord("
        << static_cast<const Token&>(id)
        << ", "
        << NAMEOF_ENUM(id.kind)
        << ")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const BasedLiteral& lit)
{
    const std::string_view fraction_part {
        lit.fraction_part.has_value() ?
        std::to_string(lit.fraction_part.value())
        : "NO FRACTION"
    };

    const std::string_view exponent {
        lit.exponent.has_value() ?
        std::to_string(lit.exponent.value())
        : "NO EXPONENT"
    };

    out << "BasedLiteral("
        << static_cast<const Token&>(lit)
        << ", "
        << lit.base
        << ", "
        << lit.decimal_part
        << ", "
        << fraction_part
        << ", "
        << exponent
        << ")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const DecimalLiteral& lit)
{
    const std::string_view fraction_part {
        lit.fraction_part.has_value() ?
            std::to_string(lit.fraction_part.value())
            : "NO FRACTION"
    };

    const std::string_view exponent {
        lit.exponent.has_value() ?
            std::to_string(lit.exponent.value())
            : "NO EXPONENT"
    };

    out << "DecimalLiteral("
        << static_cast<const Token&>(lit)
        << ", "
        << lit.decimal_part
        << ", "
        << fraction_part
        << ", "
        << exponent
        << ")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const Delimiter& delim)
{
    out << "Delimiter("
        << static_cast<const Token&>(delim)
        << ", "
        << NAMEOF_ENUM(delim.kind)
        << ")";

    return out;
}

} // namespace lexer