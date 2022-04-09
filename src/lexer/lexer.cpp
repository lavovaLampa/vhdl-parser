#include "lexer.hpp"

#include "lexer_defs.hpp"
#include "lexer_utils.hpp"
#include "result/result.hpp"

#include <algorithm>
#include <cassert>
#include <charconv>
#include <cstdint>
#include <cuchar>
#include <filesystem>
#include <fstream>
#include <gsl/pointers>
#include <gsl/assert>
#include <iostream>
#include <iterator>
#include <optional>
#include <ranges>
#include <string>
#include <variant>

namespace lexer {
/*!re2c
    re2c:flags:input = custom;
    re2c:api:style = free-form;
*/

template <std::equality_comparable T, T M>
class filter_adjacent_pairs {
private:
    bool prev_matched { false };

public:
    auto operator()(T c) -> bool
    {
        const bool advance_write_ptr { (c != M) || (! this->prev_matched) };
        this->prev_matched = (c == M) && (! this->prev_matched);

        return advance_write_ptr;
    }
};

static StringLiteral parse_string_literal(size_t offset, std::string_view view)
{
    Expects(view.size() >= 2);
    Expects(
        view[0] == '"'
        && view[view.size() - 1] == '"'
    );

    const char* const limit { view.data() + view.size() };
    const char* cursor { view.data() };
    const char* marker {};
    const char* ctxmarker {};

    auto string_view {
        std::string_view { view.begin() + 1, view.end() - 1 }
        | std::views::filter(filter_adjacent_pairs<char, '"'>())
    };

    StringLiteral result {
        { offset, view },
        std::string { string_view.begin(), string_view.end() },
        std::nullopt
    };

    /*!re2c
        re2c:define:YYCTYPE      = "char";
        re2c:define:YYFILL       = "true;";
        re2c:define:YYPEEK       = "*cursor";
        re2c:define:YYSKIP       = "++cursor;";
        re2c:define:YYBACKUP     = "marker = cursor;";
        re2c:define:YYRESTORE    = "cursor = marker;";
        re2c:define:YYBACKUPCTX  = "ctxmarker = cursor;";
        re2c:define:YYRESTORECTX = "cursor = ctxmarker;";
        re2c:define:YYRESTORETAG = "cursor = ${tag};";
        re2c:define:YYLESSTHAN   = "limit - cursor < @@{len}";
        re2c:define:YYSTAGP      = "@@{tag} = cursor;";
        re2c:define:YYSTAGN      = "@@{tag} = nullptr;";
        re2c:define:YYSHIFT      = "cursor += @@{shift};";
        re2c:define:YYSHIFTSTAG  = "@@{tag} += @@{shift};";

        operator_and = '"' "and" '"' ;
        operator_or = '"' "or" '"' ;
        operator_nand = '"' "nand" '"' ;
        operator_nor = '"' "nor" '"' ;
        operator_xor = '"' "xor" '"' ;
        operator_xnor = '"' "xnor" '"' ;
        operator_eq = '"' "=" '"' ;
        operator_neq = '"' "/=" '"' ;
        operator_lt = '"' "<" '"' ;
        operator_lte = '"' "<=" '"' ;
        operator_gt = '"' ">" '"' ;
        operator_gte = '"' ">=" '"' ;
        operator_sll = '"' "sll" '"' ;
        operator_srl = '"' "srl" '"' ;
        operator_sla = '"' "sla" '"' ;
        operator_sra = '"' "sra" '"' ;
        operator_rol = '"' "rol" '"' ;
        operator_ror = '"' "ror" '"' ;
        operator_plus = '"' "+" '"' ;
        operator_minus = '"' "-" '"' ;
        operator_ampersand = '"' "&" '"' ;
        operator_mul = '"' "*" '"' ;
        operator_div = '"' "/" '"' ;
        operator_mod = '"' "mod" '"' ;
        operator_rem = '"' "rem" '"' ;
        operator_exp = '"' "**" '"' ;
        operator_abs = '"' "abs" '"' ;
        operator_not = '"' "not" '"' ;

        operator_and        { result.operator_symbol = OpSymbol::op_and; return result; }
        operator_or         { result.operator_symbol = OpSymbol::op_or; return result; }
        operator_nand       { result.operator_symbol = OpSymbol::nand; return result; }
        operator_nor        { result.operator_symbol = OpSymbol::nor; return result; }
        operator_xor        { result.operator_symbol = OpSymbol::op_xor; return result; }
        operator_xnor       { result.operator_symbol = OpSymbol::xnor; return result; }
        operator_eq         { result.operator_symbol = OpSymbol::eq; return result; }
        operator_neq        { result.operator_symbol = OpSymbol::neq; return result; }
        operator_lt         { result.operator_symbol = OpSymbol::lt; return result; }
        operator_lte        { result.operator_symbol = OpSymbol::lte; return result; }
        operator_gt         { result.operator_symbol = OpSymbol::gt; return result; }
        operator_gte        { result.operator_symbol = OpSymbol::gte; return result; }
        operator_sll        { result.operator_symbol = OpSymbol::sll; return result; }
        operator_srl        { result.operator_symbol = OpSymbol::srl; return result; }
        operator_sla        { result.operator_symbol = OpSymbol::sla; return result; }
        operator_sra        { result.operator_symbol = OpSymbol::sra; return result; }
        operator_rol        { result.operator_symbol = OpSymbol::rol; return result; }
        operator_ror        { result.operator_symbol = OpSymbol::ror; return result; }
        operator_plus       { result.operator_symbol = OpSymbol::plus; return result; }
        operator_minus      { result.operator_symbol = OpSymbol::minus; return result; }
        operator_ampersand  { result.operator_symbol = OpSymbol::ampersand; return result; }
        operator_mul        { result.operator_symbol = OpSymbol::mul; return result; }
        operator_div        { result.operator_symbol = OpSymbol::div; return result; }
        operator_mod        { result.operator_symbol = OpSymbol::mod; return result; }
        operator_rem        { result.operator_symbol = OpSymbol::rem; return result; }
        operator_exp        { result.operator_symbol = OpSymbol::exp; return result; }
        operator_abs        { result.operator_symbol = OpSymbol::abs; return result; }
        operator_not        { result.operator_symbol = OpSymbol::op_not; return result; }

        * { return result; }
    */

    return result;
}

static auto parse_based_literal(
    size_t offset,
    std::string_view view
) noexcept -> cpp::result<BasedLiteral, ParseError>
{
    auto split_view {
        view
        | std::views::filter([](char cr) { return cr != '_'; })
        | std::views::transform([](unsigned char c) { return std::tolower(c); })
        | std::views::split('#')
        | std::views::transform([](auto view) {
              auto iter = view | std::views::common;
              return std::string { iter.begin(), iter.end() };
          })
    };

    auto split_view_iter { split_view.begin() };

    const std::string str_base { *split_view_iter };
    const std::string str_integer { *(++split_view_iter) };
    const std::string str_exponent { *(++split_view_iter) };

    // Based integer base part cannot be empty
    assert(! str_base.empty());
    // Based integer integer part cannot be empty
    assert(! str_integer.empty());

    auto integer_split {
        str_integer
        | std::views::split('.')
        | std::views::transform([](auto view) {
              auto iter = view | std::views::common;
              return std::string { iter.begin(), iter.end() };
          })
    };

    auto integer_split_iter { integer_split.begin() };

    const std::string str_decimal_part { *integer_split_iter };
    const std::string str_fractional_part { *(++integer_split_iter) };

    // Decimal part cannot be empty
    assert(! str_decimal_part.empty());

    int32_t base {};
    auto base_result { parse_int32_t(str_base) };
    if (base_result.has_error()) {
        return cpp::failure(ParseError {
            base_result.error().kind,
            "Unable to parse based integer range"
        });
    }
    base = base_result.value();
    if (! (2 <= base <= 16)) {
        return cpp::failure(ParseError {
            parse_error_kind::out_of_range,
            "Based integer base is out of range (must be between <2, 16>)"
        });
    }

    int32_t decimal_part {};
    auto decimal_result { parse_int32_t(str_decimal_part, base) };
    if (decimal_result.has_error()) {
        return cpp::failure(ParseError {
            decimal_result.error().kind,
            "Unable to parse integer part of the based integer"
        });
    }
    decimal_part = decimal_result.value();

    std::optional<int32_t> fractional_part { std::nullopt };
    if (! str_fractional_part.empty()) {
        auto fractional_result { parse_int32_t(str_fractional_part, base) };
        if (fractional_result.has_error()) {
            return cpp::failure(ParseError {
                fractional_result.error().kind,
                "Unable to parse fractional part of the based integer"
            });
        }
        fractional_part = fractional_result.value();
    }

    std::optional<int32_t> exponent { std::nullopt };
    if (! str_exponent.empty()) {
        std::string str_exponent_clean { str_exponent };

        if (str_exponent.starts_with('e')) {
            str_exponent_clean = str_exponent.substr(1);
        }

        auto exponent_result { parse_int32_t(str_exponent_clean, 10) };
        if (exponent_result.has_error()) {
            return cpp::failure(ParseError {
                exponent_result.error().kind,
                "Unable to parse exponent of the based integer"
            });
        }
        exponent = exponent_result.value();
    }

    return BasedLiteral {
        { offset,
            view },
        base,
        decimal_part,
        fractional_part,
        exponent
    };
}

static auto parse_character_literal(
    size_t offset,
    std::string_view view
) noexcept -> cpp::result<CharacterLiteral, ParseError>
{
    // TODO: Support UTF-8?
    char chr { view[1] };

    return CharacterLiteral {
        { offset, view },
        chr
    };
}

static auto parse_bitstring_literal(
    size_t offset,
    std::string_view view
) noexcept -> cpp::result<BitStringLiteral, ParseError>
{
    Expects(view.size() >= 3);
    Expects(view[1] == '"');
    Expects(view[view.size() - 1] == '"');
    Expects(
        std::tolower(static_cast<unsigned char>(view[0])) == 'b'
        || std::tolower(static_cast<unsigned char>(view[0])) == 'x'
        || std::tolower(static_cast<unsigned char>(view[0])) == 'o'
    );

    auto filtered_view {
        view
        | std::views::filter([](char c) { return c != '_'; })
        | std::views::transform([](unsigned char c) { return std::tolower(c); })
        | std::views::common
    };

    // TODO: Why create intermediate string?
    std::string lower_string { filtered_view.begin(), filtered_view.end() };

    BitStringBase base { BitStringBase::binary };
    switch (lower_string[0]) {
        case 'x': base = BitStringBase::hexadecimal; break;
        case 'o': base = BitStringBase::octal; break;
        case 'b': base = BitStringBase::binary; break;
    }

    return BitStringLiteral {
        { offset, view },
        base,
        std::string { lower_string, 2, lower_string.size() - 3 }
    };
}

static auto parse_decimal_literal(
    size_t offset,
    std::string_view view
) noexcept -> cpp::result<DecimalLiteral, ParseError>
{
    Expects(view.size() >= 1);
    Expects(std::ranges::all_of(
        view,
        [](unsigned char c) {
            return (
                c == '_'
                || c == '.'
                || std::tolower(c) == 'e'
                || c == '+'
                || c == '-'
                || std::isdigit(c)
            );
        }
    ));

    auto filtered_view {
        view
        | std::views::filter([](char c) { return c != '_'; })
        | std::views::transform([](unsigned char c) { return std::tolower(c); })
        | std::views::split('e')
        | std::views::transform([](auto view) {
            auto iter = view | std::views::common;
            return std::string { iter.begin(), iter.end() };
        })
    };

    auto view_iter { filtered_view.begin() };

    const std::string str_integer { *view_iter };
    const std::string str_exponent { *(++view_iter) };

    auto integer_view {
        str_integer
        | std::views::split('.')
        | std::views::transform([](auto view) {
            auto iter = view | std::views::common;
            return std::string { iter.begin(), iter.end() };
        })
    };

    auto integer_iter { integer_view.begin() };

    const std::string str_decimal_part { *integer_iter };
    const std::string str_fraction_part { *(++integer_iter) };

    assert(! str_decimal_part.empty());

    int32_t decimal_part { 0 };
    auto decimal_result { parse_int32_t(str_decimal_part) };
    if (! decimal_result) {
        return cpp::failure(ParseError {
            decimal_result.error().kind,
            "Unable to parse decimal part of a decimal literal"
        });
    }
    decimal_part = decimal_result.value();

    std::optional<int32_t> fraction_part { std::nullopt };
    if (! str_fraction_part.empty()) {
        auto fraction_part_result { parse_int32_t(str_fraction_part) };
        if (fraction_part_result.has_error()) {
            return cpp::failure(ParseError {
                fraction_part_result.error().kind,
                "Unable to parse fractional part of a decimal literal"
            });
        }
        fraction_part = fraction_part_result.value();
    }

    std::optional<int32_t> exponent { std::nullopt };
    if (! str_exponent.empty()) {
        auto exponent_result { parse_int32_t(str_exponent) };
        if (exponent_result.has_error()) {
            return cpp::failure(ParseError {
                exponent_result.error().kind,
                "Unable to parse exponent of a decimal literal"
            });
        }
        exponent = exponent_result.value();
    }

    return DecimalLiteral {
        { offset, view },
        decimal_part,
        fraction_part,
        exponent
    };
}

static auto parse_basic_identifier(
    size_t offset,
    std::string_view view
) noexcept -> cpp::result<BasicIdentifier, ParseError>
{
    auto lower_view {
        view
        | std::views::transform([](unsigned char c) { return std::tolower(c); })
        | std::views::common
    };

    return BasicIdentifier {
        { offset, view },
        std::string { lower_view.begin(), lower_view.end() }
    };
}

static auto parse_extended_identifier(
    size_t offset,
    std::string_view view
) noexcept -> cpp::result<ExtendedIdentifier, ParseError>
{
    Expects(view.size() >= 3);
    Expects(
        (view[0] == '\\')
        && (view[view.size() - 1] == '\\')
    );

    auto filter_view {
        std::string_view { view.begin() + 1, view.end() - 1 }
        | std::views::filter(filter_adjacent_pairs<char, '\\'>())
    };

    return ExtendedIdentifier {
        { offset, view },
        std::string { filter_view.begin(), filter_view.end() }
    };
}

static auto parse_comment(
    size_t offset,
    std::string_view view
) noexcept -> cpp::result<Comment, ParseError>
{
    Expects(view.size() >= 2);
    Expects(view.substr(0, 2) == "--");

    return Comment {
        { offset, view },
        // Skip start of comment
        std::string { view.substr(2) }
    };
}

auto lex(LexerState& state) -> cpp::result<Token, ParseError>
{
    const char* const base { state.base };
    const char* const limit { state.limit };

    auto& cursor { state.cursor };
    const char* marker {};
    const char* ctxmarker {};

    for (;;) {
        const char* const begin_cursor { state.cursor };
        const size_t begin_offset { static_cast<size_t>(state.cursor - state.base) };
        const auto reserved_fn {
            [&](ReservedWordKind kind) {
                return ReservedWord { {
                    begin_offset,
                    std::string_view {
                        begin_cursor,
                        static_cast<size_t>(cursor - begin_cursor)
                    }
                },
                kind
            }; }
        };

        /*!re2c
            re2c:define:YYCTYPE      = "char";
            re2c:define:YYFILL       = "true;";
            re2c:define:YYPEEK       = "*cursor";
            re2c:define:YYSKIP       = "++cursor;";
            re2c:define:YYBACKUP     = "marker = cursor;";
            re2c:define:YYRESTORE    = "cursor = marker;";
            re2c:define:YYBACKUPCTX  = "ctxmarker = cursor;";
            re2c:define:YYRESTORECTX = "cursor = ctxmarker;";
            re2c:define:YYRESTORETAG = "cursor = ${tag};";
            re2c:define:YYLESSTHAN   = "limit - cursor < @@{len}";
            re2c:define:YYSTAGP      = "@@{tag} = cursor;";
            re2c:define:YYSTAGN      = "@@{tag} = nullptr;";
            re2c:define:YYSHIFT      = "cursor += @@{shift};";
            re2c:define:YYSHIFTSTAG  = "@@{tag} += @@{shift};";

            digit = [0-9] ;
            integer = digit ('_'? digit)* ;
            hex_digit = [0-9a-fA-F] ;
            hex_integer = hex_digit ('_'? hex_digit)* ;
            exponent = 'e' ('+' | '-')? integer ;

            letter = [a-zA-Z] ;
            letter_or_digit = letter | digit ;

            delimiter = (" "|" "|"\n"|"\t"|"\v"|"\r"|"\f")+ ;

            comment = '--' (.*) ;
            bitstring_literal = ('b'|'o'|'x') '"' hex_integer? '"' ;
            character_literal = "'" . "'" ;
            string_literal = '"' ( . | '""' )* '"' ;
            basic_identifier = letter ('_'? letter_or_digit)* ;
            extended_identifier = "\\" ( . | "\\\\" )+ "\\" ;
            based_literal = integer '#' hex_integer ('.' hex_integer)? '#' exponent? ;
            decimal_literal = integer ('.' integer)? exponent? ;

            reserved_abs = 'abs' ;
            reserved_access = 'access' ;
            reserved_after = 'after' ;
            reserved_alias = 'alias' ;
            reserved_all = 'all' ;
            reserved_and = 'and' ;
            reserved_architecture = 'architecture' ;
            reserved_array = 'array' ;
            reserved_assert = 'assert' ;
            reserved_begin = 'begin' ;
            reserved_block = 'block' ;
            reserved_body =  'body' ;
            reserved_buffer = 'buffer' ;
            reserved_bus = 'bus' ;
            reserved_case = 'case' ;
            reserved_component = 'component' ;
            reserved_configuration = 'configuration' ;
            reserved_constant = 'constant' ;
            reserved_disconnect = 'disconnect' ;
            reserved_downto = 'downto' ;
            reserved_else = 'else' ;
            reserved_elsif = 'elsif' ;
            reserved_end = 'end' ;
            reserved_entity = 'entity' ;
            reserved_exit = 'exit' ;
            reserved_file = 'file' ;
            reserved_for = 'for' ;
            reserved_function = 'function' ;
            reserved_generate = 'generate' ;
            reserved_generic = 'generic' ;
            reserved_group = 'group' ;
            reserved_guarded = 'guarded' ;
            reserved_if = 'if' ;
            reserved_impure = 'impure' ;
            reserved_in = 'in' ;
            reserved_inertial = 'inertial' ;
            reserved_inout = 'inout' ;
            reserved_is = 'is' ;
            reserved_label = 'label' ;
            reserved_library = 'library' ;
            reserved_linkage = 'linkage' ;
            reserved_literal = 'literal' ;
            reserved_loop = 'loop' ;
            reserved_map = 'map' ;
            reserved_mod = 'mod' ;
            reserved_nand = 'nand' ;
            reserved_new = 'new' ;
            reserved_next = 'next' ;
            reserved_nor = 'nor' ;
            reserved_not = 'not' ;
            reserved_null = 'null' ;
            reserved_of = 'of' ;
            reserved_on = 'on' ;
            reserved_open = 'open' ;
            reserved_or =  'or' ;
            reserved_others = 'others' ;
            reserved_out = 'out' ;
            reserved_package = 'package' ;
            reserved_port = 'port' ;
            reserved_postponed = 'postponed' ;
            reserved_procedural = 'procedural' ;
            reserved_procedure = 'procedure' ;
            reserved_process = 'process' ;
            reserved_protected = 'protected' ;
            reserved_pure = 'pure' ;
            reserved_range = 'range' ;
            reserved_record = 'record' ;
            reserved_reference = 'reference' ;
            reserved_register = 'register' ;
            reserved_reject = 'reject' ;
            reserved_rem = 'rem' ;
            reserved_report = 'report' ;
            reserved_return = 'return' ;
            reserved_rol = 'rol' ;
            reserved_ror = 'ror' ;
            reserved_select = 'select' ;
            reserved_severity = 'severity' ;
            reserved_signal = 'signal' ;
            reserved_shared = 'shared' ;
            reserved_sla = 'sla' ;
            reserved_sll = 'sll' ;
            reserved_sra = 'sra' ;
            reserved_srl = 'srl' ;
            reserved_subtype = 'subtype' ;
            reserved_then =  'then' ;
            reserved_to = 'to' ;
            reserved_transport = 'transport' ;
            reserved_type = 'type' ;
            reserved_unaffected = 'unaffected' ;
            reserved_units = 'units' ;
            reserved_until = 'until' ;
            reserved_use = 'use' ;
            reserved_variable = 'variable' ;
            reserved_wait = 'wait' ;
            reserved_when = 'when' ;
            reserved_while = 'while' ;
            reserved_with = 'with' ;
            reserved_xnor = 'xnor' ;
            reserved_xor = 'xor' ;

            delim_arrow = "=>" ;
            delim_box = "<>" ;
            delim_double_star = "**" ;
            delim_gt_or_equal = ">=" ;
            delim_inequality = "/=" ;
            delim_lt_or_equal = "<=" ;
            delim_var_assignment = ":=" ;

            delim_ampersand = "&" ;
            delim_apostrophe = "'" ;
            delim_asterisk = "*" ;
            delim_colon = ":" ;
            delim_comma = "," ;
            delim_dot = "." ;
            delim_equals_sign = "=" ;
            delim_greater_than_sign = ">" ;
            delim_hyphen = "-" ;
            delim_left_paren = "(" ;
            delim_left_square_bracket = "[" ;
            delim_less_than_sign = "<" ;
            delim_plus_sign = "+" ;
            delim_right_paren = ")" ;
            delim_right_square_bracket = "]" ;
            delim_semicolon = ";" ;
            delim_slash = "/" ;
            delim_vertical_line = "|" ;

            delimiter           { continue; }

            reserved_abs        { return reserved_fn(ReservedWordKind::ABS); }
            reserved_access     {
                                    return ReservedWord {
                                        begin_offset,
                                        std::string_view { begin_cursor, cursor - begin_cursor },
                                        ReservedWordKind::ACCESS
                                    };
                                }
            reserved_after      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::AFTER }; }
            reserved_alias      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ALIAS }; }
            reserved_all        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ALL }; }
            reserved_and        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::AND }; }
            reserved_architecture    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ARCHITECTURE }; }
            reserved_array      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ARRAY }; }
            reserved_assert     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ASSERT }; }
            reserved_begin      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BEGIN }; }
            reserved_block      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BLOCK }; }
            reserved_body       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BODY }; }
            reserved_buffer     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BUFFER }; }
            reserved_bus        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BUS }; }
            reserved_case       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::CASE }; }
            reserved_component  { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::COMPONENT }; }
            reserved_configuration   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::CONFIGURATION }; }
            reserved_constant   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::CONSTANT }; }
            reserved_disconnect { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::DISCONNECT }; }
            reserved_downto     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::DOWNTO }; }
            reserved_else       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ELSE }; }
            reserved_elsif      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ELSIF }; }
            reserved_end        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::END }; }
            reserved_entity     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ENTITY }; }
            reserved_exit       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::EXIT }; }
            reserved_file       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::FILE }; }
            reserved_for        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::FOR }; }
            reserved_function   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::FUNCTION }; }
            reserved_generate   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::GENERATE }; }
            reserved_generic    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::GENERIC }; }
            reserved_group      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::GROUP }; }
            reserved_guarded    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::GUARDED }; }
            reserved_if         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::IF }; }
            reserved_impure     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::IMPURE }; }
            reserved_in         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::IN }; }
            reserved_inertial   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::INERTIAL }; }
            reserved_inout      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::INOUT }; }
            reserved_is         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::IS }; }
            reserved_label      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LABEL }; }
            reserved_library    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LIBRARY }; }
            reserved_linkage    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LINKAGE }; }
            reserved_literal    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LITERAL }; }
            reserved_loop       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LOOP }; }
            reserved_map        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::MAP }; }
            reserved_mod        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::MOD }; }
            reserved_nand       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NAND }; }
            reserved_new        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NEW }; }
            reserved_next       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NEXT }; }
            reserved_nor        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NOR }; }
            reserved_not        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NOT }; }
            reserved_null       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ID_NULL }; }
            reserved_of         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OF }; }
            reserved_on         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ON }; }
            reserved_open       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OPEN }; }
            reserved_or         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OR }; }
            reserved_others     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OTHERS }; }
            reserved_out        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OUT }; }
            reserved_package    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PACKAGE }; }
            reserved_port       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PORT }; }
            reserved_postponed  { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::POSTPONED }; }
            reserved_procedural { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PROCEDURAL }; }
            reserved_procedure  { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PROCEDURE }; }
            reserved_process    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PROCESS }; }
            reserved_protected  { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PROTECTED }; }
            reserved_pure       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PURE }; }
            reserved_range      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::RANGE }; }
            reserved_record     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::RECORD }; }
            reserved_reference  { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REFERENCE }; }
            reserved_register   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REGISTER }; }
            reserved_reject     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REJECT }; }
            reserved_rem        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REM }; }
            reserved_report     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REPORT }; }
            reserved_return     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::RETURN }; }
            reserved_rol        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ROL }; }
            reserved_ror        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ROR }; }
            reserved_select     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SELECT }; }
            reserved_severity   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SEVERITY }; }
            reserved_signal     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SIGNAL }; }
            reserved_shared     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SHARED }; }
            reserved_sla        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SLA }; }
            reserved_sll        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SLL }; }
            reserved_sra        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SRA }; }
            reserved_srl        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SRL }; }
            reserved_subtype    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SUBTYPE }; }
            reserved_then       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::THEN }; }
            reserved_to         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::TO }; }
            reserved_transport  { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::TRANSPORT }; }
            reserved_type       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::TYPE }; }
            reserved_unaffected { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::UNAFFECTED }; }
            reserved_units      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::UNITS }; }
            reserved_until      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::UNTIL }; }
            reserved_use        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::USE }; }
            reserved_variable   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::VARIABLE }; }
            reserved_wait       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::WAIT }; }
            reserved_when       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::WHEN }; }
            reserved_while      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::WHILE }; }
            reserved_with       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::WITH }; }
            reserved_xnor       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::XNOR }; }
            reserved_xor        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::XOR }; }

            comment                     { return parse_comment(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            bitstring_literal           { return parse_bitstring_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            character_literal           { return parse_character_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            string_literal              { return parse_string_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            basic_identifier            { return parse_basic_identifier(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            extended_identifier         { return parse_extended_identifier(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            based_literal               { return parse_based_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            decimal_literal             { return parse_decimal_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }

            delim_arrow                 { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::arrow }; }
            delim_box                   { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::box }; }
            delim_double_star           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::double_star }; }
            delim_gt_or_equal           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::gt_or_equal }; }
            delim_inequality            { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::inequality }; }
            delim_lt_or_equal           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::lt_or_equal }; }
            delim_var_assignment        { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::var_assignment }; }

            delim_ampersand             { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::ampersand }; }
            delim_apostrophe            { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::apostrophe }; }
            delim_asterisk              { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::asterisk }; }
            delim_colon                 { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::colon }; }
            delim_comma                 { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::comma }; }
            delim_dot                   { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::dot }; }
            delim_equals_sign           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::equals_sign }; }
            delim_greater_than_sign     { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::greater_than_sign }; }
            delim_hyphen                { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::hyphen }; }
            delim_left_paren            { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::left_paren }; }
            delim_left_square_bracket   { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::left_square_bracket }; }
            delim_less_than_sign        { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::less_than_sign }; }
            delim_plus_sign             { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::plus_sign }; }
            delim_right_paren           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::right_paren }; }
            delim_right_square_bracket  { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::right_square_bracket }; }
            delim_semicolon             { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::semicolon }; }
            delim_slash                 { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::slash }; }
            delim_vertical_line         { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::vertical_line }; }

            * {
                return cpp::failure(ParseError {
                    parse_error_kind::invalid_argument,
                    "Unable to recognize current token"
                });
            }
         */
    }
}

} // namespace lexer