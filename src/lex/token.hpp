#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <variant>
#include <cstdint>
#include "error.hpp"

namespace lexer {

enum class delimiter_kind {
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
enum class reserved_word_kind {
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

enum class op_symbol {
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

enum class bit_string_base {
    binary,
    octal,
    hexadecimal,
    delimiter
};

enum class token_kind {
    based_literal,
    bitstring_literal,
    character_literal,
    comment,
    decimal_literal,
    delimiter,
    identifier,
    reserved_word,
    string_literal,
};

enum class identifier_kind {
    basic,
    extended
};

class Token {
protected:
    Token(size_t offset, size_t len, token_kind kind);

public:
    token_kind kind;
    size_t offset;
    size_t len;

    virtual ~Token() = default;
};

class Comment : public Token {
public:
    std::string val;

    Comment(size_t offset, size_t len, std::string val);
};

class BitStringLiteral : public Token {
public:
    bit_string_base base;
    std::string val;

    BitStringLiteral(size_t offset, size_t len, bit_string_base base, std::string val);
};

class CharacterLiteral : public Token {
public:
    // FIXME: Proper unicode support (i.e, to char32_t?)
    char8_t val;

    CharacterLiteral(size_t offset, size_t len, char8_t val);
};

class StringLiteral : public Token {
public:
    std::string val;
    std::optional<op_symbol> operator_symbol;

    StringLiteral(
        size_t offset,
        size_t len,
        std::string val,
        std::optional<op_symbol> operator_symbol);
};

class Identifier : public Token {
public:
    identifier_kind kind;
    std::string val;

    Identifier(size_t offset, size_t len, identifier_kind kind, std::string val);
};

class ReservedWord : public Token {
public:
    reserved_word_kind kind;

    ReservedWord(size_t offset, size_t len, reserved_word_kind kind);
};

class BasedLiteral : public Token {
public:
    int32_t base;
    int32_t decimal_part;
    std::optional<int32_t> fraction_part { std::nullopt };
    std::optional<int32_t> exponent { std::nullopt };

    BasedLiteral(
        size_t offset,
        size_t len,
        int32_t base,
        int32_t decimal_part,
        std::optional<int32_t> fraction_part = std::nullopt,
        std::optional<int32_t> exponent = std::nullopt);
};

class DecimalLiteral : public Token {
public:
    int32_t decimal_part;
    std::optional<int32_t> fraction_part { std::nullopt };
    std::optional<int32_t> exponent { std::nullopt };

    DecimalLiteral(
        size_t offset,
        size_t len,
        int32_t decimal_part,
        std::optional<int32_t> fraction_part = std::nullopt,
        std::optional<int32_t> exponent = std::nullopt);
};

class Delimiter : public Token {
public:
    delimiter_kind kind;

    Delimiter(size_t offset, size_t len, delimiter_kind kind);
};


using TokenVariant = std::variant<
    Comment,
    BitStringLiteral,
    CharacterLiteral,
    StringLiteral,
    Identifier,
    ReservedWord,
    BasedLiteral,
    DecimalLiteral,
    Delimiter>;

std::ostream& operator<<(std::ostream& out, const Token& lex);

std::ostream& operator<<(std::ostream& out, const Comment& comment);
std::ostream& operator<<(std::ostream& out, const BitStringLiteral& lit);
std::ostream& operator<<(std::ostream& out, const CharacterLiteral& lit);
std::ostream& operator<<(std::ostream& out, const StringLiteral& lit);
std::ostream& operator<<(std::ostream& out, const Identifier& id);
std::ostream& operator<<(std::ostream& out, const ReservedWord& id);
std::ostream& operator<<(std::ostream& out, const BasedLiteral& lit);
std::ostream& operator<<(std::ostream& out, const DecimalLiteral& lit);
std::ostream& operator<<(std::ostream& out, const Delimiter& delim);

} // namespace lexer
