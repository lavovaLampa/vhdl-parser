export module lexer_data;

enum class DelimiterKind {
    ARROW,
    BOX,
    DOUBLE_STAR,
    GT_OR_EQUAL,
    INEQUALITY,
    LT_OR_EQUAL,
    VAR_ASSIGNMENT,

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

enum class ReservedWordKind {
    ABS,
    ACCESS,
    AFTER,
    ALIAS,
    ALL,
    AND,
    ARCHITECTURE,
    ARRAY,
    ASSERT,
    ATTRIBUTE,
    BEGIN,
    BLOCK,
    BODY,
    BUFFER,
    BUS,
    CASE,
    COMPONENT,
    CONFIGURATION,
    CONSTANT,
    DISCONNECT,
    DOWNTO,
    ELSE,
    ELSIF,
    END,
    ENTITY,
    EXIT,
    FILE,
    FOR,
    FUNCTION,
    GENERATE,
    GENERIC,
    GROUP,
    GUARDED,
    IF,
    IMPURE,
    IN,
    INERTIAL,
    INOUT,
    IS,
    LABEL,
    LIBRARY,
    LINKAGE,
    LITERAL,
    LOOP,
    MAP,
    MOD,
    NAND,
    NEW,
    NEXT,
    NOR,
    NOT,
    ID_NULL,
    OF,
    ON,
    OPEN,
    OR,
    OTHERS,
    OUT,
    PACKAGE,
    PORT,
    POSTPONED,
    PROCEDURAL,
    PROCEDURE,
    PROCESS,
    PROTECTED,
    PURE,
    RANGE,
    RECORD,
    REFERENCE,
    REGISTER,
    REJECT,
    REM,
    REPORT,
    RETURN,
    ROL,
    ROR,
    SELECT,
    SEVERITY,
    SIGNAL,
    SHARED,
    SLA,
    SLL,
    SRA,
    SRL,
    SUBTYPE,
    THEN,
    TO,
    TRANSPORT,
    TYPE,
    UNAFFECTED,
    UNITS,
    UNTIL,
    USE,
    VARIABLE,
    WAIT,
    WHEN,
    WHILE,
    WITH,
    XNOR,
    XOR,
};

std::ostream& operator<<(std::ostream& out, const ReservedWordKind& kind)
{
    const std::string_view str_map[] {
        "ABS",
        "ACCESS",
        "AFTER",
        "ALIAS",
        "ALL",
        "AND",
        "ARCHITECTURE",
        "ARRAY",
        "ASSERT",
        "ATTRIBUTE",
        "BEGIN",
        "BLOCK",
        "BODY",
        "BUFFER",
        "BUS",
        "CASE",
        "COMPONENT",
        "CONFIGURATION",
        "CONSTANT",
        "DISCONNECT",
        "DOWNTO",
        "ELSE",
        "ELSIF",
        "END",
        "ENTITY",
        "EXIT",
        "FILE",
        "FOR",
        "FUNCTION",
        "GENERATE",
        "GENERIC",
        "GROUP",
        "GUARDED",
        "IF",
        "IMPURE",
        "IN",
        "INERTIAL",
        "INOUT",
        "IS",
        "LABEL",
        "LIBRARY",
        "LINKAGE",
        "LITERAL",
        "LOOP",
        "MAP",
        "MOD",
        "NAND",
        "NEW",
        "NEXT",
        "NOR",
        "NOT",
        "ID_NULL",
        "OF",
        "ON",
        "OPEN",
        "OR",
        "OTHERS",
        "OUT",
        "PACKAGE",
        "PORT",
        "POSTPONED",
        "PROCEDURAL",
        "PROCEDURE",
        "PROCESS",
        "PROTECTED",
        "PURE",
        "RANGE",
        "RECORD",
        "REFERENCE",
        "REGISTER",
        "REJECT",
        "REM",
        "REPORT",
        "RETURN",
        "ROL",
        "ROR",
        "SELECT",
        "SEVERITY",
        "SIGNAL",
        "SHARED",
        "SLA",
        "SLL",
        "SRA",
        "SRL",
        "SUBTYPE",
        "THEN",
        "TO",
        "TRANSPORT",
        "TYPE",
        "UNAFFECTED",
        "UNITS",
        "UNTIL",
        "USE",
        "VARIABLE",
        "WAIT",
        "WHEN",
        "WHILE",
        "WITH",
        "XNOR",
        "XOR",
    };

    out << str_map[static_cast<int>(kind)];

    return out;
}

struct Lexeme {
    const size_t offset;
    std::string_view raw_view;
};

struct Comment : Lexeme {
    std::string val;
};

std::ostream& operator<<(std::ostream& out, const Comment& comment)
{
    return (out << "Comment(" << comment.offset << ", \"" << comment.raw_view << "\")");
}

enum class BitStringBase {
    BINARY,
    OCTAL,
    HEXADECIMAL
};

struct BitStringLiteral : Lexeme {
    BitStringBase base;
    std::string val;
};

std::ostream& operator<<(std::ostream& out, const BitStringLiteral& lit)
{
    return (out << "BitStringLiteral(" << lit.offset << ", \"" << lit.raw_view << "\")");
}

struct CharacterLiteral : Lexeme {
    char32_t val;
};

std::ostream& operator<<(std::ostream& out, const CharacterLiteral& lit)
{
    return (out << "CharacterLiteral(" << lit.offset << ", \"" << lit.raw_view << "\")");
}

struct StringLiteral : Lexeme {
    std::string val;
};

std::ostream& operator<<(std::ostream& out, const StringLiteral& lit)
{
    return (out << "StringLiteral(" << lit.offset << ", \"" << lit.raw_view << "\")");
}

struct BasicIdentifier : Lexeme {
    std::string val;
};

std::ostream& operator<<(std::ostream& out, const BasicIdentifier& id)
{
    return (out << "BasicIdentifier(" << id.offset << ", \"" << id.raw_view << "\")");
}

struct ExtendedIdentifier : Lexeme {
    std::string val;
};

std::ostream& operator<<(std::ostream& out, const ExtendedIdentifier& id)
{
    return (out << "ExtendedIdentifier(" << id.offset << ", \"" << id.raw_view << "\")");
}

struct ReservedWord : Lexeme {
    ReservedWordKind kind;
};

std::ostream& operator<<(std::ostream& out, const ReservedWord& id)
{
    out << "ReservedWord(" << id.offset << ", \"" << id.raw_view << "\", " << id.kind << ")";

    return out;
}

struct BasedLiteral : Lexeme {
    uint8_t base;
    uint64_t decimal_part;
    std::optional<uint64_t> fraction_part { std::nullopt };
    int64_t exponent { 1 };
};

std::ostream& operator<<(std::ostream& out, const BasedLiteral& lit)
{
    return (out << "BasedLiteral(" << lit.offset << ", \"" << lit.raw_view << "\")");
}

struct DecimalLiteral : Lexeme {
    uint64_t decimal_part;
    std::optional<uint64_t> fraction_part { std::nullopt };
    int64_t exponent { 1 };

    std::ostream& operator<<(std::ostream& out)
    {
        return (out << "BitStringLiteral(" << this->offset << ", \"" << this->raw_view << "\")");
    }
};

std::ostream& operator<<(std::ostream& out, const DecimalLiteral& lit)
{
    return (out << "DecimalLiteral(" << lit.offset << ", \"" << lit.raw_view << "\")");
}

struct Delimiter : Lexeme {
    DelimiterKind kind;
};

std::ostream& operator<<(std::ostream& out, const Delimiter& delim)
{
    return (out << "Delimiter(" << delim.offset << ", \"" << delim.raw_view << "\", " << delim.kind << ")");
}