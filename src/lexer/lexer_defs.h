#ifndef LEXER_DATA
#define LEXER_DATA

#include <iostream>

namespace Lexer {

enum class DelimiterKind {
    // Compound delimiters (2 characters)
    ARROW,
    BOX,
    DOUBLE_STAR,
    GT_OR_EQUAL,
    INEQUALITY,
    LT_OR_EQUAL,
    VAR_ASSIGNMENT,

    // Simple delimiters (1 character)
    AMPERSAND,
    APOSTROPHE,
    ASTERISK,
    COLON,
    COMMA,
    DOT,
    EQUALS_SIGN,
    GREATER_THAN_SIGN,
    HYPHEN,
    LEFT_PAREN,
    LEFT_SQUARE_BRACKET,
    LESS_THAN_SIGN,
    PLUS_SIGN,
    RIGHT_PAREN,
    RIGHT_SQUARE_BRACKET,
    SEMICOLON,
    SLASH,
    VERTICAL_LINE
};

// clang-format off
enum class ReservedWordKind {
    ABS, ACCESS, AFTER, ALIAS, ALL, AND, ARCHITECTURE, ARRAY, ASSERT, ATTRIBUTE,
    BEGIN, BLOCK, BODY, BUFFER, BUS,
    CASE, COMPONENT, CONFIGURATION, CONSTANT,
    DISCONNECT, DOWNTO,
    ELSE, ELSIF, END, ENTITY, EXIT,
    FILE, FOR, FUNCTION,
    GENERATE, GENERIC, GROUP, GUARDED,
    IF, IMPURE, IN, INERTIAL, INOUT, IS,
    LABEL, LIBRARY, LINKAGE, LITERAL, LOOP,
    MAP, MOD,
    NAND, NEW, NEXT, NOR, NOT, ID_NULL,
    OF, ON, OPEN, OR, OTHERS, OUT,
    PACKAGE, PORT, POSTPONED, PROCEDURAL, PROCEDURE, PROCESS, PROTECTED, PURE,
    RANGE, RECORD, REFERENCE, REGISTER, REJECT, REM, REPORT, RETURN, ROL, ROR,
    SELECT, SEVERITY, SIGNAL, SHARED, SLA, SLL, SRA, SRL, SUBTYPE,
    THEN, TO, TRANSPORT, TYPE,
    UNAFFECTED, UNITS, UNTIL, USE,
    VARIABLE,
    WAIT, WHEN, WHILE, WITH,
    XNOR, XOR,
};
// clang-format on

enum class BitStringBase {
    BINARY,
    OCTAL,
    HEXADECIMAL
};

struct Lexeme {
    const size_t offset;
    std::string_view raw_view;
};

struct Comment : Lexeme {
    std::string val;
};

struct BitStringLiteral : Lexeme {
    BitStringBase base;
    std::string val;
};

struct CharacterLiteral : Lexeme {
    char32_t val;
};

struct StringLiteral : Lexeme {
    std::string val;
};

struct BasicIdentifier : Lexeme {
    std::string val;
};

struct ExtendedIdentifier : Lexeme {
    std::string val;
};

struct ReservedWord : Lexeme {
    ReservedWordKind kind;
};

struct BasedLiteral : Lexeme {
    uint8_t base;
    uint64_t decimal_part;
    std::optional<uint64_t> fraction_part { std::nullopt };
    int64_t exponent { 1 };
};

struct DecimalLiteral : Lexeme {
    uint64_t decimal_part;
    std::optional<uint64_t> fraction_part { std::nullopt };
    int64_t exponent { 1 };
};

struct Delimiter : Lexeme {
    DelimiterKind kind;
};

using Token = std::variant<
    Comment,
    BitStringLiteral,
    CharacterLiteral,
    StringLiteral,
    ReservedWord,
    BasicIdentifier,
    ExtendedIdentifier,
    BasedLiteral,
    DecimalLiteral,
    Delimiter>;

std::ostream& operator<<(std::ostream& out, const DelimiterKind& kind);
std::ostream& operator<<(std::ostream& out, const ReservedWordKind& kind);
std::ostream& operator<<(std::ostream& out, const Comment& comment);
std::ostream& operator<<(std::ostream& out, const BitStringLiteral& lit);
std::ostream& operator<<(std::ostream& out, const CharacterLiteral& lit);
std::ostream& operator<<(std::ostream& out, const StringLiteral& lit);
std::ostream& operator<<(std::ostream& out, const BasicIdentifier& id);
std::ostream& operator<<(std::ostream& out, const ExtendedIdentifier& id);
std::ostream& operator<<(std::ostream& out, const ReservedWord& id);
std::ostream& operator<<(std::ostream& out, const BasedLiteral& lit);
std::ostream& operator<<(std::ostream& out, const DecimalLiteral& lit);
std::ostream& operator<<(std::ostream& out, const Delimiter& delim);

} // namespace Lexer

#endif /* LEXER_DATA */
