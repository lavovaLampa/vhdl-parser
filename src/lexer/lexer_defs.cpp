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
        "ARROW",
        "BOX",
        "DOUBLE_STAR",
        "GT_OR_EQUAL",
        "INEQUALITY",
        "LT_OR_EQUAL",
        "VAR_ASSIGNMENT",

        "AMPERSAND",
        "APOSTROPHE",
        "ASTERISK",
        "COLON",
        "COMMA",
        "DOT",
        "EQUALS_SIGN",
        "GREATER_THAN_SIGN",
        "HYPHEN",
        "LEFT_PAREN",
        "LEFT_SQUARE_BRACKET",
        "LESS_THAN_SIGN",
        "PLUS_SIGN",
        "RIGHT_PAREN",
        "RIGHT_SQUARE_BRACKET",
        "SEMICOLON",
        "SLASH",
        "VERTICAL_LINE",
    };

    out << delimiter_str_map[static_cast<int>(kind)];

    return out;
}

std::ostream& operator<<(std::ostream& out, const ReservedWordKind& kind)
{
    // clang-format off
    const std::string_view str_map[] {
        "ABS", "ACCESS", "AFTER", "ALIAS", "ALL", "AND", "ARCHITECTURE", "ARRAY", "ASSERT", "ATTRIBUTE",
        "BEGIN", "BLOCK", "BODY", "BUFFER", "BUS",
        "CASE", "COMPONENT", "CONFIGURATION", "CONSTANT",
        "DISCONNECT", "DOWNTO",
        "ELSE", "ELSIF", "END", "ENTITY", "EXIT",
        "FILE", "FOR", "FUNCTION",
        "GENERATE", "GENERIC", "GROUP", "GUARDED",
        "IF", "IMPURE", "IN", "INERTIAL", "INOUT", "IS",
        "LABEL", "LIBRARY", "LINKAGE", "LITERAL", "LOOP",
        "MAP", "MOD",
        "NAND", "NEW", "NEXT", "NOR", "NOT", "ID_NULL",
        "OF", "ON", "OPEN", "OR", "OTHERS", "OUT",
        "PACKAGE", "PORT", "POSTPONED", "PROCEDURAL", "PROCEDURE", "PROCESS", "PROTECTED", "PURE",
        "RANGE", "RECORD", "REFERENCE", "REGISTER", "REJECT", "REM", "REPORT", "RETURN", "ROL", "ROR",
        "SELECT", "SEVERITY", "SIGNAL", "SHARED", "SLA", "SLL", "SRA", "SRL", "SUBTYPE",
        "THEN", "TO", "TRANSPORT", "TYPE",
        "UNAFFECTED", "UNITS", "UNTIL", "USE",
        "VARIABLE",
        "WAIT", "WHEN", "WHILE", "WITH",
        "XNOR", "XOR",
    };
    // clang-format on

    out << str_map[static_cast<int>(kind)];

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
    out << "StringLiteral("
        << lit.offset
        << ", \""
        << lit.raw_view
        << "\")";

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