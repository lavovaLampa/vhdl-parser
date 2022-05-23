#include "pre_parse_lexer.hpp"

#include "token.hpp"
#include "common.hpp"

#include <cassert>
#include <gsl/assert>

namespace lexer {

tl::optional<TokenVariant> lex_pre_parse(std::string_view src, const char* cursor)
{
    Expects((src.size() == 0) || *(src.end() - 1) == '\0');

    const char* const base { src.begin() };
    const char* const sentinel { src.end() };

    assert(base <= sentinel);

    const char* cursor { cursor };
    const char* token_begin { cursor };

    const char* marker {};
    const char* ctxmarker {};

    auto make_reserved { [&](reserved_word_kind kind) {
        return ReservedWord {
            static_cast<size_t>(token_begin - base),
            static_cast<size_t>(cursor - token_begin),
            kind
        };
    } };

    auto make_delimiter{ [&](delimiter_kind kind) {
        return Delimiter { 
            static_cast<size_t>(token_begin - base),
            static_cast<size_t>(cursor - token_begin),
            kind
        };
    } };

    while (true) {
        token_begin = cursor;

        assert(cursor <= sentinel);
        assert(token_begin >= base);
        assert(cursor >= token_begin);


        /*!include:re2c "./re2c_common.txt" */

        /*!re2c
            re2c:flags:input = custom;
            re2c:api:style = free-form;

            re2c:eof = 0;
            re2c:yyfill:enable = 0;

            re2c:define:YYCTYPE      = "char";
            re2c:define:YYPEEK       = "*cursor";
            re2c:define:YYSKIP       = "++cursor;";
            re2c:define:YYBACKUP     = "marker = cursor;";
            re2c:define:YYRESTORE    = "cursor = marker;";
            re2c:define:YYBACKUPCTX  = "ctxmarker = cursor;";
            re2c:define:YYRESTORECTX = "cursor = ctxmarker;";
            re2c:define:YYRESTORETAG = "cursor = ${tag};";
            re2c:define:YYLESSTHAN   = "sentinel - cursor < @@{len}";
            re2c:define:YYSTAGP      = "@@{tag} = cursor;";
            re2c:define:YYSTAGN      = "@@{tag} = NULL;";
            re2c:define:YYSHIFT      = "cursor += @@{shift};";
            re2c:define:YYSHIFTSTAG  = "@@{tag} += @@{shift};";

            comment             { continue; }
            bitstring_literal   { continue; }
            character_literal   { continue; }
            string_literal      { continue; }
            based_literal       { continue; }
            decimal_literal     { continue; }

            reserved_abs            { continue; }
            reserved_access         { continue; }
            reserved_after          { continue; }
            reserved_alias          { continue; }
            reserved_all            { continue; }
            reserved_and            { continue; }
            reserved_array          { continue; }
            reserved_assert         { continue; }
            reserved_buffer         { continue; }
            reserved_bus            { continue; }
            reserved_constant       { continue; }
            reserved_disconnect     { continue; }
            reserved_downto         { continue; }
            reserved_else           { continue; }
            reserved_elsif          { continue; }
            reserved_exit           { continue; }
            reserved_file           { continue; }
            reserved_generic        { continue; }
            reserved_group          { continue; }
            reserved_guarded        { continue; }
            reserved_impure         { continue; }
            reserved_inertial       { continue; }
            reserved_inout          { continue; }
            reserved_is             { continue; }
            reserved_label          { continue; }
            reserved_library        { continue; }
            reserved_linkage        { continue; }
            reserved_literal        { continue; }
            reserved_map            { continue; }
            reserved_mod            { continue; }
            reserved_nand           { continue; }
            reserved_new            { continue; }
            reserved_next           { continue; }
            reserved_nor            { continue; }
            reserved_not            { continue; }
            reserved_null           { continue; }
            reserved_of             { continue; }
            reserved_on             { continue; }
            reserved_open           { continue; }
            reserved_or             { continue; }
            reserved_others         { continue; }
            reserved_out            { continue; }
            reserved_port           { continue; }
            reserved_postponed      { continue; }
            reserved_procedural     { continue; }
            reserved_pure           { continue; }
            reserved_range          { continue; }
            reserved_reference      { continue; }
            reserved_register       { continue; }
            reserved_reject         { continue; }
            reserved_rem            { continue; }
            reserved_report         { continue; }
            reserved_return         { continue; }
            reserved_rol            { continue; }
            reserved_ror            { continue; }
            reserved_select         { continue; }
            reserved_severity       { continue; }
            reserved_signal         { continue; }
            reserved_shared         { continue; }
            reserved_sla            { continue; }
            reserved_sll            { continue; }
            reserved_sra            { continue; }
            reserved_srl            { continue; }
            reserved_subtype        { continue; }
            reserved_then           { continue; }
            reserved_to             { continue; }
            reserved_transport      { continue; }
            reserved_type           { continue; }
            reserved_unaffected     { continue; }
            reserved_until          { continue; }
            reserved_use            { continue; }
            reserved_variable       { continue; }
            reserved_wait           { continue; }
            reserved_when           { continue; }
            reserved_while          { continue; }
            reserved_with           { continue; }
            reserved_xnor           { continue; }
            reserved_xor            { continue; }

            reserved_architecture   { make_reserved(reserved_word_kind::ARCHITECTURE); }
            reserved_begin          { make_reserved(reserved_word_kind::BEGIN); }
            reserved_block          { make_reserved(reserved_word_kind::BLOCK); }
            reserved_body           { make_reserved(reserved_word_kind::BODY); }
            reserved_case           { make_reserved(reserved_word_kind::CASE); }
            reserved_component      { make_reserved(reserved_word_kind::COMPONENT); }
            reserved_configuration  { make_reserved(reserved_word_kind::CONFIGURATION); }
            reserved_end            { make_reserved(reserved_word_kind::END); }
            reserved_entity         { make_reserved(reserved_word_kind::ENTITY); }
            reserved_for            { make_reserved(reserved_word_kind::FOR); }
            reserved_function       { make_reserved(reserved_word_kind::FUNCTION); }
            reserved_generate       { make_reserved(reserved_word_kind::GENERATE); }
            reserved_if             { make_reserved(reserved_word_kind::IF); }
            reserved_in             { make_reserved(reserved_word_kind::IN); }
            reserved_loop           { make_reserved(reserved_word_kind::LOOP); }
            reserved_package        { make_reserved(reserved_word_kind::PACKAGE); }
            reserved_procedure      { make_reserved(reserved_word_kind::PROCEDURE); }
            reserved_process        { make_reserved(reserved_word_kind::PROCESS); }
            reserved_protected      { make_reserved(reserved_word_kind::PROTECTED); }
            reserved_record         { make_reserved(reserved_word_kind::RECORD); }
            reserved_units          { make_reserved(reserved_word_kind::UNITS); }

            delim_arrow                 { continue; }
            delim_box                   { continue; }
            delim_double_star           { continue; }
            delim_gt_or_equal           { continue; }
            delim_inequality            { continue; }
            delim_lt_or_equal           { continue; }
            delim_var_assignment        { continue; }

            delim_ampersand             { continue; }
            delim_apostrophe            { continue; }
            delim_asterisk              { continue; }
            delim_colon                 { continue; }
            delim_comma                 { continue; }
            delim_dot                   { continue; }
            delim_equals_sign           { continue; }
            delim_greater_than_sign     { continue; }
            delim_hyphen                { continue; }
            delim_left_paren            { continue; }
            delim_left_square_bracket   { continue; }
            delim_less_than_sign        { continue; }
            delim_plus_sign             { continue; }
            delim_right_paren           { continue; }
            delim_right_square_bracket  { continue; }
            delim_semicolon             { make_delimiter(delimiter_kind::semicolon); }
            delim_slash                 { continue; }
            delim_vertical_line         { continue; }

            
            basic_identifier    {
                return parse_basic_identifier(
                    static_cast<size_t>(token_begin - base),
                    make_string_view()
                );
            }
            extended_identifier {
                return parse_extended_identifier(
                    static_cast<size_t>(token_begin - base),
                    make_string_view()
                );
            }

            $   { return std::nullopt; }

            *   {
                // TODO: Improve exception and/or use result type
                throw "Invalid token detected";
            }

        */
    }
}

} // namespace lexer