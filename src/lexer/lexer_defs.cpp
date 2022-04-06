#include "lexer_defs.hpp"

#include <cstdint>
#include <cuchar>
#include <iostream>
#include <optional>
#include <variant>

namespace Lexer {

std::ostream& operator<<(std::ostream& out, DelimiterKind kind)
{
    switch (kind) {
        case DelimiterKind::arrow: out << "=>"; break;
        case DelimiterKind::box: out << "<>"; break;
        case DelimiterKind::double_star: out << "**"; break;
        case DelimiterKind::gt_or_equal: out << ">="; break;
        case DelimiterKind::inequality: out << "/="; break;
        case DelimiterKind::lt_or_equal: out << "<="; break;
        case DelimiterKind::var_assignment: out << ":="; break;
        case DelimiterKind::ampersand: out << "&"; break;
        case DelimiterKind::apostrophe: out << "'"; break;
        case DelimiterKind::asterisk: out << "*"; break;
        case DelimiterKind::colon: out << ":"; break;
        case DelimiterKind::comma: out << ","; break;
        case DelimiterKind::dot: out << "."; break;
        case DelimiterKind::equals_sign: out << "="; break;
        case DelimiterKind::greater_than_sign: out << ">"; break;
        case DelimiterKind::hyphen: out << "-"; break;
        case DelimiterKind::left_paren: out << "("; break;
        case DelimiterKind::left_square_bracket: out << "["; break;
        case DelimiterKind::less_than_sign: out << "<"; break;
        case DelimiterKind::plus_sign: out << "+"; break;
        case DelimiterKind::right_paren: out << ")"; break;
        case DelimiterKind::right_square_bracket: out << "]"; break;
        case DelimiterKind::semicolon: out << ";"; break;
        case DelimiterKind::slash: out << "/"; break;
        case DelimiterKind::vertical_line: out << "|"; break;
        default: std::abort();
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, ReservedWordKind kind)
{
    switch (kind) {
        case ReservedWordKind::ABS: out << "abs"; break;
        case ReservedWordKind::ACCESS: out << "access"; break;
        case ReservedWordKind::AFTER: out << "after"; break;
        case ReservedWordKind::ALIAS: out << "alias"; break;
        case ReservedWordKind::ALL: out << "all"; break;
        case ReservedWordKind::AND: out << "and"; break;
        case ReservedWordKind::ARCHITECTURE: out << "architecture"; break;
        case ReservedWordKind::ARRAY: out << "array"; break;
        case ReservedWordKind::ASSERT: out << "assert"; break;
        case ReservedWordKind::ATTRIBUTE: out << "attribute"; break;
        case ReservedWordKind::BEGIN: out << "begin"; break;
        case ReservedWordKind::BLOCK: out << "block"; break;
        case ReservedWordKind::BODY: out << "body"; break;
        case ReservedWordKind::BUFFER: out << "buffer"; break;
        case ReservedWordKind::BUS: out << "bus"; break;
        case ReservedWordKind::CASE: out << "case"; break;
        case ReservedWordKind::COMPONENT: out << "component"; break;
        case ReservedWordKind::CONFIGURATION: out << "configuration"; break;
        case ReservedWordKind::CONSTANT: out << "constant"; break;
        case ReservedWordKind::DISCONNECT: out << "disconnect"; break;
        case ReservedWordKind::DOWNTO: out << "downto"; break;
        case ReservedWordKind::ELSE: out << "else"; break;
        case ReservedWordKind::ELSIF: out << "elsif"; break;
        case ReservedWordKind::END: out << "end"; break;
        case ReservedWordKind::ENTITY: out << "entity"; break;
        case ReservedWordKind::EXIT: out << "exit"; break;
        case ReservedWordKind::FILE: out << "file"; break;
        case ReservedWordKind::FOR: out << "for"; break;
        case ReservedWordKind::FUNCTION: out << "function"; break;
        case ReservedWordKind::GENERATE: out << "generate"; break;
        case ReservedWordKind::GENERIC: out << "generic"; break;
        case ReservedWordKind::GROUP: out << "group"; break;
        case ReservedWordKind::GUARDED: out << "guarded"; break;
        case ReservedWordKind::IF: out << "if"; break;
        case ReservedWordKind::IMPURE: out << "impure"; break;
        case ReservedWordKind::IN: out << "in"; break;
        case ReservedWordKind::INERTIAL: out << "inertial"; break;
        case ReservedWordKind::INOUT: out << "inout"; break;
        case ReservedWordKind::IS: out << "is"; break;
        case ReservedWordKind::LABEL: out << "label"; break;
        case ReservedWordKind::LIBRARY: out << "library"; break;
        case ReservedWordKind::LINKAGE: out << "linkage"; break;
        case ReservedWordKind::LITERAL: out << "literal"; break;
        case ReservedWordKind::LOOP: out << "loop"; break;
        case ReservedWordKind::MAP: out << "map"; break;
        case ReservedWordKind::MOD: out << "mod"; break;
        case ReservedWordKind::NAND: out << "nand"; break;
        case ReservedWordKind::NEW: out << "new"; break;
        case ReservedWordKind::NEXT: out << "next"; break;
        case ReservedWordKind::NOR: out << "nor"; break;
        case ReservedWordKind::NOT: out << "not"; break;
        case ReservedWordKind::ID_NULL: out << "null"; break;
        case ReservedWordKind::OF: out << "of"; break;
        case ReservedWordKind::ON: out << "on"; break;
        case ReservedWordKind::OPEN: out << "open"; break;
        case ReservedWordKind::OR: out << "or"; break;
        case ReservedWordKind::OTHERS: out << "others"; break;
        case ReservedWordKind::OUT: out << "out"; break;
        case ReservedWordKind::PACKAGE: out << "package"; break;
        case ReservedWordKind::PORT: out << "port"; break;
        case ReservedWordKind::POSTPONED: out << "postponed"; break;
        case ReservedWordKind::PROCEDURAL: out << "procedural"; break;
        case ReservedWordKind::PROCEDURE: out << "procedure"; break;
        case ReservedWordKind::PROCESS: out << "process"; break;
        case ReservedWordKind::PROTECTED: out << "protected"; break;
        case ReservedWordKind::PURE: out << "pure"; break;
        case ReservedWordKind::RANGE: out << "range"; break;
        case ReservedWordKind::RECORD: out << "record"; break;
        case ReservedWordKind::REFERENCE: out << "reference"; break;
        case ReservedWordKind::REGISTER: out << "register"; break;
        case ReservedWordKind::REJECT: out << "reject"; break;
        case ReservedWordKind::REM: out << "rem"; break;
        case ReservedWordKind::REPORT: out << "report"; break;
        case ReservedWordKind::RETURN: out << "return"; break;
        case ReservedWordKind::ROL: out << "rol"; break;
        case ReservedWordKind::ROR: out << "ror"; break;
        case ReservedWordKind::SELECT: out << "select"; break;
        case ReservedWordKind::SEVERITY: out << "severity"; break;
        case ReservedWordKind::SIGNAL: out << "signal"; break;
        case ReservedWordKind::SHARED: out << "shared"; break;
        case ReservedWordKind::SLA: out << "sla"; break;
        case ReservedWordKind::SLL: out << "sll"; break;
        case ReservedWordKind::SRA: out << "sra"; break;
        case ReservedWordKind::SRL: out << "srl"; break;
        case ReservedWordKind::SUBTYPE: out << "subtype"; break;
        case ReservedWordKind::THEN: out << "then"; break;
        case ReservedWordKind::TO: out << "to"; break;
        case ReservedWordKind::TRANSPORT: out << "transport"; break;
        case ReservedWordKind::TYPE: out << "type"; break;
        case ReservedWordKind::UNAFFECTED: out << "unaffected"; break;
        case ReservedWordKind::UNITS: out << "units"; break;
        case ReservedWordKind::UNTIL: out << "until"; break;
        case ReservedWordKind::USE: out << "use"; break;
        case ReservedWordKind::VARIABLE: out << "variable"; break;
        case ReservedWordKind::WAIT: out << "wait"; break;
        case ReservedWordKind::WHEN: out << "when"; break;
        case ReservedWordKind::WHILE: out << "while"; break;
        case ReservedWordKind::WITH: out << "with"; break;
        case ReservedWordKind::XNOR: out << "xnor"; break;
        case ReservedWordKind::XOR: out << "xor"; break;
        default: std::abort();
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, OpSymbol op)
{
    switch (op) {
        case OpSymbol::op_and: out << "and"; break;
        case OpSymbol::op_or: out << "or"; break;
        case OpSymbol::nand: out << "nand"; break;
        case OpSymbol::nor: out << "nor"; break;
        case OpSymbol::op_xor: out << "xor"; break;
        case OpSymbol::xnor: out << "xnor"; break;
        case OpSymbol::eq: out << "="; break;
        case OpSymbol::neq: out << "/="; break;
        case OpSymbol::lt: out << "<"; break;
        case OpSymbol::lte: out << "<="; break;
        case OpSymbol::gt: out << ">"; break;
        case OpSymbol::gte: out << "<="; break;
        case OpSymbol::sll: out << "sll"; break;
        case OpSymbol::srl: out << "srl"; break;
        case OpSymbol::sla: out << "sla"; break;
        case OpSymbol::sra: out << "sra"; break;
        case OpSymbol::rol: out << "rol"; break;
        case OpSymbol::ror: out << "ror"; break;
        case OpSymbol::plus: out << "+"; break;
        case OpSymbol::minus: out << "-"; break;
        case OpSymbol::ampersand: out << "&"; break;
        case OpSymbol::mul: out << "*"; break;
        case OpSymbol::div: out << "/"; break;
        case OpSymbol::mod: out << "mod"; break;
        case OpSymbol::rem: out << "rem"; break;
        case OpSymbol::exp: out << "exp"; break;
        case OpSymbol::abs: out << "abs"; break;
        case OpSymbol::op_not: out << "not"; break;
        default: std::abort();
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, BitStringBase base)
{
    switch (base) {
        case BitStringBase::binary: out << "binary"; break;
        case BitStringBase::hexadecimal: out << "hexadecimal"; break;
        case BitStringBase::octal: out << "octal"; break;
        default: std::abort();
    }

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
        std::to_string(lit.fraction_part.value())
        : "NO FRACTION"
    };

    const std::string_view exponent {
        lit.exponent.has_value() ?
        std::to_string(lit.exponent.value())
        : "NO EXPONENT"
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
            std::to_string(lit.fraction_part.value())
            : "NO FRACTION"
    };

    const std::string_view exponent {
        lit.exponent.has_value() ?
            std::to_string(lit.exponent.value())
            : "NO EXPONENT"
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