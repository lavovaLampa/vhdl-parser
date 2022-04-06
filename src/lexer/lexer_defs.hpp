#ifndef LEXER_DATA
#define LEXER_DATA

#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace Lexer {

enum class DelimiterKind {
    // Compound delimiters (2 characters)
    arrow,
    box,
    double_star,
    gt_or_equal,
    inequality,
    lt_or_equal,
    var_assignment,

    // Simple delimiters (1 character)
    ampersand,
    apostrophe,
    asterisk,
    colon,
    comma,
    dot,
    equals_sign,
    greater_than_sign,
    hyphen,
    left_paren,
    left_square_bracket,
    less_than_sign,
    plus_sign,
    right_paren,
    right_square_bracket,
    semicolon,
    slash,
    vertical_line
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

enum class OpSymbol {
    op_and,
    op_or,
    nand,
    nor,
    op_xor,
    xnor,
    eq,
    neq,
    lt,
    lte,
    gt,
    gte,
    sll,
    srl,
    sla,
    sra,
    rol,
    ror,
    plus,
    minus,
    ampersand,
    mul,
    div,
    mod,
    rem,
    exp,
    abs,
    op_not
};

enum class BitStringBase {
    binary,
    octal,
    hexadecimal
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
    char val;
};

struct StringLiteral : Lexeme {
    std::string val;
    std::optional<OpSymbol> operator_symbol;
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
    int32_t base;
    int32_t decimal_part;
    std::optional<int32_t> fraction_part { std::nullopt };
    std::optional<int32_t> exponent { std::nullopt };
};

struct DecimalLiteral : Lexeme {
    int32_t decimal_part;
    std::optional<int32_t> fraction_part { std::nullopt };
    std::optional<int32_t> exponent { std::nullopt };
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

std::ostream& operator<<(std::ostream& out, const Lexeme& lex);

std::ostream& operator<<(std::ostream& out, const BitStringBase base);
std::ostream& operator<<(std::ostream& out, const DelimiterKind kind);
std::ostream& operator<<(std::ostream& out, const ReservedWordKind kind);
std::ostream& operator<<(std::ostream& out, const OpSymbol op);

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
