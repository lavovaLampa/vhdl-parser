#include <cstdint>
#include <cuchar>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <variant>

#include "lexer_defs.h"
#include "lexer.h"

namespace Lexer {

std::optional<Token> lex(LexerState& state)
{
    const char* const base { state.base };
    const char* const limit { state.limit };

    auto& cursor { state.cursor };
    const char* marker {};
    const char* ctxmarker {};

    for (;;) {
        const char* const begin_cursor { state.cursor };
        const size_t begin_offset { state.cursor - state.base };

        /*!re2c
            re2c:flags:input = custom;
            re2c:api:style = free-form;

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

            delimiter                   { continue; }

            reserved_abs         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ABS }; }
            reserved_access      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ACCESS }; }
            reserved_after       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::AFTER }; }
            reserved_alias       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ALIAS }; }
            reserved_all         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ALL }; }
            reserved_and         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::AND }; }
            reserved_architecture    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ARCHITECTURE }; }
            reserved_array       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ARRAY }; }
            reserved_assert      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ASSERT }; }
            reserved_begin       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BEGIN }; }
            reserved_block       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BLOCK }; }
            reserved_body        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BODY }; }
            reserved_buffer      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BUFFER }; }
            reserved_bus         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::BUS }; }
            reserved_case        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::CASE }; }
            reserved_component   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::COMPONENT }; }
            reserved_configuration   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::CONFIGURATION }; }
            reserved_constant    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::CONSTANT }; }
            reserved_disconnect  { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::DISCONNECT }; }
            reserved_downto      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::DOWNTO }; }
            reserved_else        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ELSE }; }
            reserved_elsif       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ELSIF }; }
            reserved_end         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::END }; }
            reserved_entity      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ENTITY }; }
            reserved_exit        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::EXIT }; }
            reserved_file        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::FILE }; }
            reserved_for         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::FOR }; }
            reserved_function    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::FUNCTION }; }
            reserved_generate    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::GENERATE }; }
            reserved_generic     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::GENERIC }; }
            reserved_group       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::GROUP }; }
            reserved_guarded     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::GUARDED }; }
            reserved_if          { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::IF }; }
            reserved_impure      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::IMPURE }; }
            reserved_in          { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::IN }; }
            reserved_inertial    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::INERTIAL }; }
            reserved_inout       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::INOUT }; }
            reserved_is          { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::IS }; }
            reserved_label       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LABEL }; }
            reserved_library     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LIBRARY }; }
            reserved_linkage     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LINKAGE }; }
            reserved_literal     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LITERAL }; }
            reserved_loop        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::LOOP }; }
            reserved_map         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::MAP }; }
            reserved_mod         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::MOD }; }
            reserved_nand        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NAND }; }
            reserved_new         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NEW }; }
            reserved_next        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NEXT }; }
            reserved_nor         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NOR }; }
            reserved_not         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::NOT }; }
            reserved_null        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ID_NULL }; }
            reserved_of          { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OF }; }
            reserved_on          { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ON }; }
            reserved_open        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OPEN }; }
            reserved_or          { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OR }; }
            reserved_others      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OTHERS }; }
            reserved_out         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::OUT }; }
            reserved_package     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PACKAGE }; }
            reserved_port        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PORT }; }
            reserved_postponed   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::POSTPONED }; }
            reserved_procedural  { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PROCEDURAL }; }
            reserved_procedure   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PROCEDURE }; }
            reserved_process     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PROCESS }; }
            reserved_protected   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PROTECTED }; }
            reserved_pure        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::PURE }; }
            reserved_range       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::RANGE }; }
            reserved_record      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::RECORD }; }
            reserved_reference   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REFERENCE }; }
            reserved_register    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REGISTER }; }
            reserved_reject      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REJECT }; }
            reserved_rem         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REM }; }
            reserved_report      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::REPORT }; }
            reserved_return      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::RETURN }; }
            reserved_rol         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ROL }; }
            reserved_ror         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::ROR }; }
            reserved_select      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SELECT }; }
            reserved_severity    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SEVERITY }; }
            reserved_signal      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SIGNAL }; }
            reserved_shared      { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SHARED }; }
            reserved_sla         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SLA }; }
            reserved_sll         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SLL }; }
            reserved_sra         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SRA }; }
            reserved_srl         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SRL }; }
            reserved_subtype     { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::SUBTYPE }; }
            reserved_then        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::THEN }; }
            reserved_to          { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::TO }; }
            reserved_transport   { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::TRANSPORT }; }
            reserved_type        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::TYPE }; }
            reserved_unaffected  { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::UNAFFECTED }; }
            reserved_units       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::UNITS }; }
            reserved_until       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::UNTIL }; }
            reserved_use         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::USE }; }
            reserved_variable    { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::VARIABLE }; }
            reserved_wait        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::WAIT }; }
            reserved_when        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::WHEN }; }
            reserved_while       { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::WHILE }; }
            reserved_with        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::WITH }; }
            reserved_xnor        { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::XNOR }; }
            reserved_xor         { return ReservedWord { begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }, ReservedWordKind::XOR }; }

            comment                     { return Comment { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, "" }; }
            bitstring_literal           { return BitStringLiteral { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, BitStringBase::BINARY, "" }; }
            character_literal           { return CharacterLiteral { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, 'a' }; }
            string_literal              { return StringLiteral { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, "" }; }
            basic_identifier            { return BasicIdentifier { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, "" }; }
            extended_identifier         { return ExtendedIdentifier { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, "" }; }
            based_literal               { return BasedLiteral { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, 0, 0 }; }
            decimal_literal             { return DecimalLiteral { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, 0 }; }

            delim_arrow                 { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::ARROW }; }
            delim_box                   { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::BOX }; }
            delim_double_star           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::DOUBLE_STAR }; }
            delim_gt_or_equal           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::GT_OR_EQUAL }; }
            delim_inequality            { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::INEQUALITY }; }
            delim_lt_or_equal           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::LT_OR_EQUAL }; }
            delim_var_assignment        { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::VAR_ASSIGNMENT }; }

            delim_ampersand             { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::AMPERSAND }; }
            delim_apostrophe            { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::APOSTROPHE }; }
            delim_asterisk              { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::ASTERISK }; }
            delim_colon                 { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::COLON }; }
            delim_comma                 { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::COMMA }; }
            delim_dot                   { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::DOT }; }
            delim_equals_sign           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::EQUALS_SIGN }; }
            delim_greater_than_sign     { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::GREATER_THAN_SIGN }; }
            delim_hyphen                { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::HYPHEN }; }
            delim_left_paren            { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::LEFT_PAREN }; }
            delim_left_square_bracket   { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::LEFT_SQUARE_BRACKET }; }
            delim_less_than_sign        { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::LESS_THAN_SIGN }; }
            delim_plus_sign             { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::PLUS_SIGN }; }
            delim_right_paren           { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::RIGHT_PAREN }; }
            delim_right_square_bracket  { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::RIGHT_SQUARE_BRACKET }; }
            delim_semicolon             { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::SEMICOLON }; }
            delim_slash                 { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::SLASH }; }
            delim_vertical_line         { return Delimiter { begin_offset, std::string_view {begin_cursor, cursor - begin_cursor}, DelimiterKind::VERTICAL_LINE }; }

            * { return std::nullopt; }
         */
    }
}

} // namespace Lexer