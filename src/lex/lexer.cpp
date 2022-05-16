#include "lexer.hpp"

#include "token.hpp"
#include "common.hpp"

#include <cassert>
#include <gsl/assert>

namespace lexer {

Lexer::Lexer(std::string_view input, int32_t lookahead)
    : lookahead { lookahead }
    , source { input }
    , cursor { input.begin( ) }
    , tokens { std::queue<std::optional<Token>>() }
{
    for (int32_t i { 0 }; i < lookahead; i++) {
        tokens.emplace(lex_pre_parse(this->source, this->cursor));
    }
}

std::optional<Token> Lexer::pop()
{
    const auto result { std::move(this->tokens.front()) };
    this->tokens.pop();
    this->tokens.emplace(lex_pre_parse(this->source, this->cursor));
    return result;
}

const std::queue<std::optional<Token>>& Lexer::peek() const noexcept
{
    return this->tokens;
}

void Lexer::reset() noexcept
{
    this->cursor = this->source.begin();
}

std::optional<Token> lex(std::string_view src, const char* cursor)
{
    Expects((src.size() == 0) || *(src.end() - 1) == '\0');

    const char* const base { src.begin() };
    const char* const sentinel { src.end() };

    const char* cursor { cursor };
    const char* token_begin { cursor };

    const char* marker {};
    const char* ctxmarker {};

    auto make_string_view { [&]() {
        return std::string_view { token_begin, cursor - token_begin };
    } };

    auto make_reserved { [&](ReservedWordKind kind) {
        Expects(cursor >= token_begin);

        return ReservedWord {
            {
                .kind { lexeme_kind::reserved_word },
                .offset { static_cast<size_t>(cursor - token_begin) },
                .raw_view { make_string_view() }
            },
            kind
        };
    } };

    auto make_delimiter { [&](delimiter_kind kind) {
        return Delimiter {
            {
                .kind { lexeme_kind::delimiter },
                .offset { static_cast<size_t>(cursor - token_begin) },
                .raw_view { make_string_view( )}
            },
            kind
        };
    } };

    while (true) {
        assert(cursor <= sentinel);
        assert(token_begin >= base);

        token_begin = cursor;

        /*!include:re2c "./re2c_common.txt" */

        /*!re2c
            re2c:flags:input = custom;
            re2c:api:style = free-form;

            re2c:eof = 0;
            re2c:yyfill:enable = 0

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

            delimiter           { continue; }

            reserved_abs            { return make_reserved(ReservedWordKind::ABS); }
            reserved_access         { return make_reserved(ReservedWordKind::ACCESS); }
            reserved_after          { return make_reserved(ReservedWordKind::AFTER); }
            reserved_alias          { return make_reserved(ReservedWordKind::ALIAS); }
            reserved_all            { return make_reserved(ReservedWordKind::ALL); }
            reserved_and            { return make_reserved(ReservedWordKind::AND); }
            reserved_architecture   { return make_reserved(ReservedWordKind::ARCHITECTURE); }
            reserved_array          { return make_reserved(ReservedWordKind::ARRAY); }
            reserved_assert         { return make_reserved(ReservedWordKind::ASSERT); }
            reserved_begin          { return make_reserved(ReservedWordKind::BEGIN); }
            reserved_block          { return make_reserved(ReservedWordKind::BLOCK); }
            reserved_body           { return make_reserved(ReservedWordKind::BODY); }
            reserved_buffer         { return make_reserved(ReservedWordKind::BUFFER); }
            reserved_bus            { return make_reserved(ReservedWordKind::BUS); }
            reserved_case           { return make_reserved(ReservedWordKind::CASE); }
            reserved_component      { return make_reserved(ReservedWordKind::COMPONENT); }
            reserved_configuration  { return make_reserved(ReservedWordKind::CONFIGURATION); }
            reserved_constant       { return make_reserved(ReservedWordKind::CONSTANT); }
            reserved_disconnect     { return make_reserved(ReservedWordKind::DISCONNECT); }
            reserved_downto         { return make_reserved(ReservedWordKind::DOWNTO); }
            reserved_else           { return make_reserved(ReservedWordKind::ELSE); }
            reserved_elsif          { return make_reserved(ReservedWordKind::ELSIF); }
            reserved_end            { return make_reserved(ReservedWordKind::END); }
            reserved_entity         { return make_reserved(ReservedWordKind::ENTITY); }
            reserved_exit           { return make_reserved(ReservedWordKind::EXIT); }
            reserved_file           { return make_reserved(ReservedWordKind::FILE); }
            reserved_for            { return make_reserved(ReservedWordKind::FOR); }
            reserved_function       { return make_reserved(ReservedWordKind::FUNCTION); }
            reserved_generate       { return make_reserved(ReservedWordKind::GENERATE); }
            reserved_generic        { return make_reserved(ReservedWordKind::GENERIC); }
            reserved_group          { return make_reserved(ReservedWordKind::GROUP); }
            reserved_guarded        { return make_reserved(ReservedWordKind::GUARDED); }
            reserved_if             { return make_reserved(ReservedWordKind::IF); }
            reserved_impure         { return make_reserved(ReservedWordKind::IMPURE); }
            reserved_in             { return make_reserved(ReservedWordKind::IN); }
            reserved_inertial       { return make_reserved(ReservedWordKind::INERTIAL); }
            reserved_inout          { return make_reserved(ReservedWordKind::INOUT); }
            reserved_is             { return make_reserved(ReservedWordKind::IS); }
            reserved_label          { return make_reserved(ReservedWordKind::LABEL); }
            reserved_library        { return make_reserved(ReservedWordKind::LIBRARY); }
            reserved_linkage        { return make_reserved(ReservedWordKind::LINKAGE); }
            reserved_literal        { return make_reserved(ReservedWordKind::LITERAL); }
            reserved_loop           { return make_reserved(ReservedWordKind::LOOP); }
            reserved_map            { return make_reserved(ReservedWordKind::MAP); }
            reserved_mod            { return make_reserved(ReservedWordKind::MOD); }
            reserved_nand           { return make_reserved(ReservedWordKind::NAND); }
            reserved_new            { return make_reserved(ReservedWordKind::NEW); }
            reserved_next           { return make_reserved(ReservedWordKind::NEXT); }
            reserved_nor            { return make_reserved(ReservedWordKind::NOR); }
            reserved_not            { return make_reserved(ReservedWordKind::NOT); }
            reserved_null           { return make_reserved(ReservedWordKind::NULL); }
            reserved_of             { return make_reserved(ReservedWordKind::OF); }
            reserved_on             { return make_reserved(ReservedWordKind::ON); }
            reserved_open           { return make_reserved(ReservedWordKind::OPEN); }
            reserved_or             { return make_reserved(ReservedWordKind::OR); }
            reserved_others         { return make_reserved(ReservedWordKind::OTHERS); }
            reserved_out            { return make_reserved(ReservedWordKind::OUT); }
            reserved_package        { return make_reserved(ReservedWordKind::PACKAGE); }
            reserved_port           { return make_reserved(ReservedWordKind::PORT); }
            reserved_postponed      { return make_reserved(ReservedWordKind::POSTPONED); }
            reserved_procedural     { return make_reserved(ReservedWordKind::PROCEDURAL); }
            reserved_procedure      { return make_reserved(ReservedWordKind::PROCEDURE); }
            reserved_process        { return make_reserved(ReservedWordKind::PROCESS); }
            reserved_protected      { return make_reserved(ReservedWordKind::PROTECTED); }
            reserved_pure           { return make_reserved(ReservedWordKind::PURE); }
            reserved_range          { return make_reserved(ReservedWordKind::RANGE); }
            reserved_record         { return make_reserved(ReservedWordKind::RECORD); }
            reserved_reference      { return make_reserved(ReservedWordKind::REFERENCE); }
            reserved_register       { return make_reserved(ReservedWordKind::REGISTER); }
            reserved_reject         { return make_reserved(ReservedWordKind::REJECT); }
            reserved_rem            { return make_reserved(ReservedWordKind::REM); }
            reserved_report         { return make_reserved(ReservedWordKind::REPORT); }
            reserved_return         { return make_reserved(ReservedWordKind::RETURN); }
            reserved_rol            { return make_reserved(ReservedWordKind::ROL); }
            reserved_ror            { return make_reserved(ReservedWordKind::ROR); }
            reserved_select         { return make_reserved(ReservedWordKind::SELECT); }
            reserved_severity       { return make_reserved(ReservedWordKind::SEVERITY); }
            reserved_signal         { return make_reserved(ReservedWordKind::SIGNAL); }
            reserved_shared         { return make_reserved(ReservedWordKind::SHARED); }
            reserved_sla            { return make_reserved(ReservedWordKind::SLA); }
            reserved_sll            { return make_reserved(ReservedWordKind::SLL); }
            reserved_sra            { return make_reserved(ReservedWordKind::SRA); }
            reserved_srl            { return make_reserved(ReservedWordKind::SRL); }
            reserved_subtype        { return make_reserved(ReservedWordKind::SUBTYPE); }
            reserved_then           { return make_reserved(ReservedWordKind::THEN); }
            reserved_to             { return make_reserved(ReservedWordKind::TO); }
            reserved_transport      { return make_reserved(ReservedWordKind::TRANSPORT); }
            reserved_type           { return make_reserved(ReservedWordKind::TYPE); }
            reserved_unaffected     { return make_reserved(ReservedWordKind::UNAFFECTED); }
            reserved_units          { return make_reserved(ReservedWordKind::UNITS); }
            reserved_until          { return make_reserved(ReservedWordKind::UNTIL); }
            reserved_use            { return make_reserved(ReservedWordKind::USE); }
            reserved_variable       { return make_reserved(ReservedWordKind::VARIABLE); }
            reserved_wait           { return make_reserved(ReservedWordKind::WAIT); }
            reserved_when           { return make_reserved(ReservedWordKind::WHEN); }
            reserved_while          { return make_reserved(ReservedWordKind::WHILE); }
            reserved_with           { return make_reserved(ReservedWordKind::WITH); }
            reserved_xnor           { return make_reserved(ReservedWordKind::XNOR); }
            reserved_xor            { return make_reserved(ReservedWordKind::XOR); }

            comment                     { return parse_comment(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            bitstring_literal           { return parse_bitstring_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            character_literal           { return parse_character_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            string_literal              { return parse_string_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            basic_identifier            { return parse_basic_identifier(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            extended_identifier         { return parse_extended_identifier(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            based_literal               { return parse_based_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }
            decimal_literal             { return parse_decimal_literal(begin_offset, std::string_view { begin_cursor, cursor - begin_cursor }); }

            delim_arrow                 { return make_delimiter(delimiter_kind::arrow); }
            delim_box                   { return make_delimiter(delimiter_kind::box); }
            delim_double_star           { return make_delimiter(delimiter_kind::double_star); }
            delim_gt_or_equal           { return make_delimiter(delimiter_kind::gt_or_equal); }
            delim_inequality            { return make_delimiter(delimiter_kind::inequality); }
            delim_lt_or_equal           { return make_delimiter(delimiter_kind::lt_or_equal); }
            delim_var_assignment        { return make_delimiter(delimiter_kind::var_assignment); }

            delim_ampersand             { return make_delimiter(delimiter_kind::ampersand) }
            delim_apostrophe            { return make_delimiter(delimiter_kind::apostrophe) }
            delim_asterisk              { return make_delimiter(delimiter_kind::asterisk) }
            delim_colon                 { return make_delimiter(delimiter_kind::colon) }
            delim_comma                 { return make_delimiter(delimiter_kind::comma) }
            delim_dot                   { return make_delimiter(delimiter_kind::dot) }
            delim_equals_sign           { return make_delimiter(delimiter_kind::equals_sign) }
            delim_greater_than_sign     { return make_delimiter(delimiter_kind::greater_than_sign) }
            delim_hyphen                { return make_delimiter(delimiter_kind::hyphen) }
            delim_left_paren            { return make_delimiter(delimiter_kind::left_paren) }
            delim_left_square_bracket   { return make_delimiter(delimiter_kind::left_square_bracket) }
            delim_less_than_sign        { return make_delimiter(delimiter_kind::less_than_sign) }
            delim_plus_sign             { return make_delimiter(delimiter_kind::plus_sign) }
            delim_right_paren           { return make_delimiter(delimiter_kind::right_paren) }
            delim_right_square_bracket  { return make_delimiter(delimiter_kind::right_square_bracket) }
            delim_semicolon             { return make_delimiter(delimiter_kind::semicolon) }
            delim_slash                 { return make_delimiter(delimiter_kind::slash) }
            delim_vertical_line         { return make_delimiter(delimiter_kind::vertical_line) }

            $ { return std::nullopt; }

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