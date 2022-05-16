#include "token.hpp"

#include <cstdint>
#include <cuchar>

namespace lexer {

std::ostream& operator<<(std::ostream& out, delimiter_kind kind)
{
    switch (kind) {
        case delimiter_kind::arrow: out << "=>"; break;
        case delimiter_kind::box: out << "<>"; break;
        case delimiter_kind::double_star: out << "**"; break;
        case delimiter_kind::gt_or_equal: out << ">="; break;
        case delimiter_kind::inequality: out << "/="; break;
        case delimiter_kind::lt_or_equal: out << "<="; break;
        case delimiter_kind::var_assignment: out << ":="; break;
        case delimiter_kind::ampersand: out << "&"; break;
        case delimiter_kind::apostrophe: out << "'"; break;
        case delimiter_kind::asterisk: out << "*"; break;
        case delimiter_kind::colon: out << ":"; break;
        case delimiter_kind::comma: out << ","; break;
        case delimiter_kind::dot: out << "."; break;
        case delimiter_kind::equals_sign: out << "="; break;
        case delimiter_kind::greater_than_sign: out << ">"; break;
        case delimiter_kind::hyphen: out << "-"; break;
        case delimiter_kind::left_paren: out << "("; break;
        case delimiter_kind::left_square_bracket: out << "["; break;
        case delimiter_kind::less_than_sign: out << "<"; break;
        case delimiter_kind::plus_sign: out << "+"; break;
        case delimiter_kind::right_paren: out << ")"; break;
        case delimiter_kind::right_square_bracket: out << "]"; break;
        case delimiter_kind::semicolon: out << ";"; break;
        case delimiter_kind::slash: out << "/"; break;
        case delimiter_kind::vertical_line: out << "|"; break;
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

std::ostream& operator<<(std::ostream& out, op_symbol op)
{
    switch (op) {
        case op_symbol::op_and: out << "and"; break;
        case op_symbol::op_or: out << "or"; break;
        case op_symbol::nand: out << "nand"; break;
        case op_symbol::nor: out << "nor"; break;
        case op_symbol::op_xor: out << "xor"; break;
        case op_symbol::xnor: out << "xnor"; break;
        case op_symbol::eq: out << "="; break;
        case op_symbol::neq: out << "/="; break;
        case op_symbol::lt: out << "<"; break;
        case op_symbol::lte: out << "<="; break;
        case op_symbol::gt: out << ">"; break;
        case op_symbol::gte: out << "<="; break;
        case op_symbol::sll: out << "sll"; break;
        case op_symbol::srl: out << "srl"; break;
        case op_symbol::sla: out << "sla"; break;
        case op_symbol::sra: out << "sra"; break;
        case op_symbol::rol: out << "rol"; break;
        case op_symbol::ror: out << "ror"; break;
        case op_symbol::plus: out << "+"; break;
        case op_symbol::minus: out << "-"; break;
        case op_symbol::ampersand: out << "&"; break;
        case op_symbol::mul: out << "*"; break;
        case op_symbol::div: out << "/"; break;
        case op_symbol::mod: out << "mod"; break;
        case op_symbol::rem: out << "rem"; break;
        case op_symbol::exp: out << "exp"; break;
        case op_symbol::abs: out << "abs"; break;
        case op_symbol::op_not: out << "not"; break;
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

} // namespace lexer