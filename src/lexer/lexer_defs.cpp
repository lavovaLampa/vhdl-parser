#include <cstdint>
#include <cuchar>
#include <iostream>
#include <optional>
#include <variant>

#include "lexer_defs.h"

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

std::ostream& operator<<(std::ostream& out, const Comment& comment)
{
    out << "Comment("
        << comment.offset
        << ", \""
        << comment.raw_view
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const BitStringLiteral& lit)
{
    out << "BitStringLiteral("
        << lit.offset
        << ", \""
        << lit.raw_view
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const CharacterLiteral& lit)
{
    out << "CharacterLiteral("
        << lit.offset
        << ", \""
        << lit.raw_view
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const StringLiteral& lit)
{
    if (lit.operator_symbol.has_value()) {
        out << "StringLiteral("
            << lit.val
            << ", "
            << lit.operator_symbol.value()
            << ", "
            << lit.offset
            << ", \""
            << lit.raw_view
            << "\")";
    } else {
        out << "StringLiteral("
            << lit.val
            << ", "
            << lit.offset
            << ", \""
            << lit.raw_view
            << "\")";
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, const BasicIdentifier& id)
{
    out << "BasicIdentifier("
        << id.offset
        << ", \""
        << id.raw_view
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const ExtendedIdentifier& id)
{
    out << "ExtendedIdentifier("
        << id.offset
        << ", \""
        << id.raw_view
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const ReservedWord& id)
{
    out << "ReservedWord("
        << id.offset
        << ", \""
        << id.raw_view
        << "\", "
        << id.kind
        << ")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const BasedLiteral& lit)
{
    out << "BasedLiteral("
        << lit.offset
        << ", \""
        << lit.raw_view
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const DecimalLiteral& lit)
{
    out << "DecimalLiteral("
        << lit.offset
        << ", \""
        << lit.raw_view
        << "\")";

    return out;
}

std::ostream& operator<<(std::ostream& out, const Delimiter& delim)
{
    out << "Delimiter("
        << delim.offset
        << ", \""
        << delim.raw_view
        << "\", "
        << delim.kind
        << ")";

    return out;
}

} // namespace Lexer