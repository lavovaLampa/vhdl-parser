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
    , tokens { std::queue<std::optional<TokenVariant>>() }
{
    for (int32_t i { 0 }; i < lookahead; i++) {
        tokens.emplace(lex_pre_parse(this->source, this->cursor));
    }
}

std::optional<TokenVariant> Lexer::pop()
{
    const auto result { std::move(this->tokens.front()) };
    this->tokens.pop();
    this->tokens.emplace(lex_pre_parse(this->source, this->cursor));
    return result;
}

const std::queue<std::optional<TokenVariant>>& Lexer::peek() const noexcept
{
    return this->tokens;
}

void Lexer::reset() noexcept
{
    this->cursor = this->source.begin();
}

std::optional<TokenVariant> lex(std::string_view src, const char* cursor)
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

    auto make_reserved { [&](reserved_word_kind kind) {
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

            reserved_abs            { return make_reserved(reserved_word_kind::ABS); }
            reserved_access         { return make_reserved(reserved_word_kind::ACCESS); }
            reserved_after          { return make_reserved(reserved_word_kind::AFTER); }
            reserved_alias          { return make_reserved(reserved_word_kind::ALIAS); }
            reserved_all            { return make_reserved(reserved_word_kind::ALL); }
            reserved_and            { return make_reserved(reserved_word_kind::AND); }
            reserved_architecture   { return make_reserved(reserved_word_kind::ARCHITECTURE); }
            reserved_array          { return make_reserved(reserved_word_kind::ARRAY); }
            reserved_assert         { return make_reserved(reserved_word_kind::ASSERT); }
            reserved_begin          { return make_reserved(reserved_word_kind::BEGIN); }
            reserved_block          { return make_reserved(reserved_word_kind::BLOCK); }
            reserved_body           { return make_reserved(reserved_word_kind::BODY); }
            reserved_buffer         { return make_reserved(reserved_word_kind::BUFFER); }
            reserved_bus            { return make_reserved(reserved_word_kind::BUS); }
            reserved_case           { return make_reserved(reserved_word_kind::CASE); }
            reserved_component      { return make_reserved(reserved_word_kind::COMPONENT); }
            reserved_configuration  { return make_reserved(reserved_word_kind::CONFIGURATION); }
            reserved_constant       { return make_reserved(reserved_word_kind::CONSTANT); }
            reserved_disconnect     { return make_reserved(reserved_word_kind::DISCONNECT); }
            reserved_downto         { return make_reserved(reserved_word_kind::DOWNTO); }
            reserved_else           { return make_reserved(reserved_word_kind::ELSE); }
            reserved_elsif          { return make_reserved(reserved_word_kind::ELSIF); }
            reserved_end            { return make_reserved(reserved_word_kind::END); }
            reserved_entity         { return make_reserved(reserved_word_kind::ENTITY); }
            reserved_exit           { return make_reserved(reserved_word_kind::EXIT); }
            reserved_file           { return make_reserved(reserved_word_kind::FILE); }
            reserved_for            { return make_reserved(reserved_word_kind::FOR); }
            reserved_function       { return make_reserved(reserved_word_kind::FUNCTION); }
            reserved_generate       { return make_reserved(reserved_word_kind::GENERATE); }
            reserved_generic        { return make_reserved(reserved_word_kind::GENERIC); }
            reserved_group          { return make_reserved(reserved_word_kind::GROUP); }
            reserved_guarded        { return make_reserved(reserved_word_kind::GUARDED); }
            reserved_if             { return make_reserved(reserved_word_kind::IF); }
            reserved_impure         { return make_reserved(reserved_word_kind::IMPURE); }
            reserved_in             { return make_reserved(reserved_word_kind::IN); }
            reserved_inertial       { return make_reserved(reserved_word_kind::INERTIAL); }
            reserved_inout          { return make_reserved(reserved_word_kind::INOUT); }
            reserved_is             { return make_reserved(reserved_word_kind::IS); }
            reserved_label          { return make_reserved(reserved_word_kind::LABEL); }
            reserved_library        { return make_reserved(reserved_word_kind::LIBRARY); }
            reserved_linkage        { return make_reserved(reserved_word_kind::LINKAGE); }
            reserved_literal        { return make_reserved(reserved_word_kind::LITERAL); }
            reserved_loop           { return make_reserved(reserved_word_kind::LOOP); }
            reserved_map            { return make_reserved(reserved_word_kind::MAP); }
            reserved_mod            { return make_reserved(reserved_word_kind::MOD); }
            reserved_nand           { return make_reserved(reserved_word_kind::NAND); }
            reserved_new            { return make_reserved(reserved_word_kind::NEW); }
            reserved_next           { return make_reserved(reserved_word_kind::NEXT); }
            reserved_nor            { return make_reserved(reserved_word_kind::NOR); }
            reserved_not            { return make_reserved(reserved_word_kind::NOT); }
            reserved_null           { return make_reserved(reserved_word_kind::NULL); }
            reserved_of             { return make_reserved(reserved_word_kind::OF); }
            reserved_on             { return make_reserved(reserved_word_kind::ON); }
            reserved_open           { return make_reserved(reserved_word_kind::OPEN); }
            reserved_or             { return make_reserved(reserved_word_kind::OR); }
            reserved_others         { return make_reserved(reserved_word_kind::OTHERS); }
            reserved_out            { return make_reserved(reserved_word_kind::OUT); }
            reserved_package        { return make_reserved(reserved_word_kind::PACKAGE); }
            reserved_port           { return make_reserved(reserved_word_kind::PORT); }
            reserved_postponed      { return make_reserved(reserved_word_kind::POSTPONED); }
            reserved_procedural     { return make_reserved(reserved_word_kind::PROCEDURAL); }
            reserved_procedure      { return make_reserved(reserved_word_kind::PROCEDURE); }
            reserved_process        { return make_reserved(reserved_word_kind::PROCESS); }
            reserved_protected      { return make_reserved(reserved_word_kind::PROTECTED); }
            reserved_pure           { return make_reserved(reserved_word_kind::PURE); }
            reserved_range          { return make_reserved(reserved_word_kind::RANGE); }
            reserved_record         { return make_reserved(reserved_word_kind::RECORD); }
            reserved_reference      { return make_reserved(reserved_word_kind::REFERENCE); }
            reserved_register       { return make_reserved(reserved_word_kind::REGISTER); }
            reserved_reject         { return make_reserved(reserved_word_kind::REJECT); }
            reserved_rem            { return make_reserved(reserved_word_kind::REM); }
            reserved_report         { return make_reserved(reserved_word_kind::REPORT); }
            reserved_return         { return make_reserved(reserved_word_kind::RETURN); }
            reserved_rol            { return make_reserved(reserved_word_kind::ROL); }
            reserved_ror            { return make_reserved(reserved_word_kind::ROR); }
            reserved_select         { return make_reserved(reserved_word_kind::SELECT); }
            reserved_severity       { return make_reserved(reserved_word_kind::SEVERITY); }
            reserved_signal         { return make_reserved(reserved_word_kind::SIGNAL); }
            reserved_shared         { return make_reserved(reserved_word_kind::SHARED); }
            reserved_sla            { return make_reserved(reserved_word_kind::SLA); }
            reserved_sll            { return make_reserved(reserved_word_kind::SLL); }
            reserved_sra            { return make_reserved(reserved_word_kind::SRA); }
            reserved_srl            { return make_reserved(reserved_word_kind::SRL); }
            reserved_subtype        { return make_reserved(reserved_word_kind::SUBTYPE); }
            reserved_then           { return make_reserved(reserved_word_kind::THEN); }
            reserved_to             { return make_reserved(reserved_word_kind::TO); }
            reserved_transport      { return make_reserved(reserved_word_kind::TRANSPORT); }
            reserved_type           { return make_reserved(reserved_word_kind::TYPE); }
            reserved_unaffected     { return make_reserved(reserved_word_kind::UNAFFECTED); }
            reserved_units          { return make_reserved(reserved_word_kind::UNITS); }
            reserved_until          { return make_reserved(reserved_word_kind::UNTIL); }
            reserved_use            { return make_reserved(reserved_word_kind::USE); }
            reserved_variable       { return make_reserved(reserved_word_kind::VARIABLE); }
            reserved_wait           { return make_reserved(reserved_word_kind::WAIT); }
            reserved_when           { return make_reserved(reserved_word_kind::WHEN); }
            reserved_while          { return make_reserved(reserved_word_kind::WHILE); }
            reserved_with           { return make_reserved(reserved_word_kind::WITH); }
            reserved_xnor           { return make_reserved(reserved_word_kind::XNOR); }
            reserved_xor            { return make_reserved(reserved_word_kind::XOR); }

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