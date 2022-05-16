#include "pre_parse_lexer.hpp"

#include "token.hpp"
#include "common.hpp"

#include <cassert>
#include <gsl/assert>

namespace lexer {
std::optional<Token> lex_pre_parse(std::string_view src, const char* cursor)
{
    Expects((src.size() == 0) || *(src.end() - 1) == '\0');

    const char* const base { src.begin() };
    const char* const sentinel { src.end() };

    assert(base <= sentinel);

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


    while (true) {
        assert(cursor <= sentinel);
        assert(token_begin >= base);

        token_begin = cursor;

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

            reserved_architecture   { make_reserved(ReservedWordKind::ARCHITECTURE); }
            reserved_begin          { make_reserved(ReservedWordKind::BEGIN); }
            reserved_block          { make_reserved(ReservedWordKind::BLOCK); }
            reserved_body           { make_reserved(ReservedWordKind::BODY); }
            reserved_case           { make_reserved(ReservedWordKind::CASE); }
            reserved_component      { make_reserved(ReservedWordKind::COMPONENT); }
            reserved_configuration  { make_reserved(ReservedWordKind::CONFIGURATION); }
            reserved_end            { make_reserved(ReservedWordKind::END); }
            reserved_entity         { make_reserved(ReservedWordKind::ENTITY); }
            reserved_for            { make_reserved(ReservedWordKind::FOR); }
            reserved_function       { make_reserved(ReservedWordKind::FUNCTION); }
            reserved_generate       { make_reserved(ReservedWordKind::GENERATE); }
            reserved_if             { make_reserved(ReservedWordKind::IF); }
            reserved_in             { make_reserved(ReservedWordKind::IN); }
            reserved_loop           { make_reserved(ReservedWordKind::LOOP); }
            reserved_package        { make_reserved(ReservedWordKind::PACKAGE); }
            reserved_procedure      { make_reserved(ReservedWordKind::PROCEDURE); }
            reserved_process        { make_reserved(ReservedWordKind::PROCESS); }
            reserved_protected      { make_reserved(ReservedWordKind::PROTECTED); }
            reserved_record         { make_reserved(ReservedWordKind::RECORD); }
            reserved_units          { make_reserved(ReservedWordKind::UNITS); }

            
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