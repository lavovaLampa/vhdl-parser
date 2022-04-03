#include <cstdint>
#include <cuchar>
#include <iostream>
#include <optional>
#include <variant>

#include "lexer_defs.hpp"

namespace Lexer {

std::ostream& operator<<(std::ostream& out, const DelimiterKind& kind)
{
    const std::string_view delimiter_str_map[] {
        "arrow",
        "box",
        "double_star",
        "gt_or_equal",
        "inequality",
        "lt_or_equal",
        "var_assignment",

        "ampersand",
        "apostrophe",
        "asterisk",
        "colon",
        "comma",
        "dot",
        "equals_sign",
        "greater_than_sign",
        "hyphen",
        "left_paren",
        "left_square_bracket",
        "less_than_sign",
        "plus_sign",
        "right_paren",
        "right_square_bracket",
        "semicolon",
        "slash",
        "vertical_line",
    };

    out << delimiter_str_map[static_cast<int>(kind)];

    return out;
}

std::ostream& operator<<(std::ostream& out, const ReservedWordKind& kind)
{
    // clang-format off
    const std::string_view str_map[] {
        "abs", "access", "after", "alias", "all", "and", "architecture", "array", "assert", "attribute",
        "begin", "block", "body", "buffer", "bus",
        "case", "component", "configuration", "constant",
        "disconnect", "downto",
        "else", "elsif", "end", "entity", "exit",
        "file", "for", "function",
        "generate", "generic", "group", "guarded",
        "if", "impure", "in", "inertial", "inout", "is",
        "label", "library", "linkage", "literal", "loop",
        "map", "mod",
        "nand", "new", "next", "nor", "not", "id_null",
        "of", "on", "open", "or", "others", "out",
        "package", "port", "postponed", "procedural", "procedure", "process", "protected", "pure",
        "range", "record", "reference", "register", "reject", "rem", "report", "return", "rol", "ror",
        "select", "severity", "signal", "shared", "sla", "sll", "sra", "srl", "subtype",
        "then", "to", "transport", "type",
        "unaffected", "units", "until", "use",
        "variable",
        "wait", "when", "while", "with",
        "xnor", "xor",
    };
    // clang-format on

    out << str_map[static_cast<int>(kind)];

    return out;
}

std::ostream& operator<<(std::ostream& out, const OpSymbol& op)
{
    const std::string_view str_map[] {
        "and",
        "or",
        "nand",
        "nor",
        "xor",
        "xnor",
        "=",
        "/=",
        "<",
        "<=",
        ">",
        ">=",
        "sll",
        "srl",
        "sla",
        "sra",
        "rol",
        "ror",
        "+",
        "-",
        "&",
        "*",
        "/",
        "mod",
        "rem",
        "**",
        "abs",
        "not",
    };

    out << str_map[static_cast<int>(op)];

    return out;
}

std::ostream& operator<<(std::ostream& out, const BitStringBase base)
{
    const std::string_view str_map[] = {
        "binary",
        "octal",
        "hexadecimal"
    };

    out << str_map[static_cast<int>(base)];

    return out;
}

std::ostream& operator<<(std::ostream& out, const Lexeme& lex)
{
    out << lex.offset
        << ", \""
        << lex.raw_view
        << "\"";

    return out;
}

std::ostream& operator<<(std::ostream& out, const Comment& comment)
{
    out << "Comment("
        << static_cast<const Lexeme&>(comment)
        << ", \""
        << comment.val
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const BitStringLiteral& lit)
{
    out << "BitStringLiteral("
        << static_cast<const Lexeme&>(lit)
        << ", "
        << lit.base
        << ", \""
        << lit.val
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const CharacterLiteral& lit)
{
    out << "CharacterLiteral("
        << static_cast<const Lexeme&>(lit)
        << ", '"
        << lit.val
        << "')";

    return out;
}

std::ostream& operator<<(std::ostream& out, const StringLiteral& lit)
{
    out << "StringLiteral("
        << static_cast<const Lexeme&>(lit)
        << ", \""
        << lit.val
        << "\"";

    if (lit.operator_symbol.has_value()) {
        out << ", "
            << lit.operator_symbol.value();
    }

    out << ")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const BasicIdentifier& id)
{
    out << "BasicIdentifier("
        << static_cast<const Lexeme&>(id)
        << ", \""
        << id.val
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const ExtendedIdentifier& id)
{
    out << "ExtendedIdentifier("
        << static_cast<const Lexeme&>(id)
        << ", \""
        << id.val
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const ReservedWord& id)
{
    out << "ReservedWord("
        << static_cast<const Lexeme&>(id)
        << ", "
        << id.kind
        << ")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const BasedLiteral& lit)
{
    const std::string_view fraction_part {
        lit.fraction_part.has_value() ?
            std::to_string(lit.fraction_part.value()) : "NO FRACTION"
    };

    const std::string_view exponent {
        lit.exponent.has_value() ?
            std::to_string(lit.exponent.value()) : "NO EXPONENT"
    };

    out << "BasedLiteral("
        << static_cast<const Lexeme&>(lit)
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
            std::to_string(lit.fraction_part.value()) : "NO FRACTION"
    };

    const std::string_view exponent {
        lit.exponent.has_value() ?
            std::to_string(lit.exponent.value()) : "NO EXPONENT"
    };

    out << "DecimalLiteral("
        << static_cast<const Lexeme&>(lit)
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
        << static_cast<const Lexeme&>(delim)
        << ", "
        << delim.kind
        << ")";

    return out;
}

} // namespace Lexer