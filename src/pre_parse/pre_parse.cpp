#include "pre_parse.hpp"

#include "lex/lexer.hpp"
#include "lex/pre_parse_lexer.hpp"
#include "lex/token.hpp"
#include "tl/optional.hpp"

#include <deque>
#include <functional>
#include <initializer_list>
#include <variant>


namespace pre_parse {

std::vector<LibraryUnit> pre_parse_file(fs::path file)
{
}

namespace parser {
    static void architecture_body(lexer::Lexer& lexer);
    static void block_or_component_configuration(lexer::Lexer& lexer);
    static void block_statement(lexer::Lexer& lexer);
    static void case_statement(lexer::Lexer& lexer);
    static void component_declaration(lexer::Lexer& lexer);
    static void configuration_declaration(lexer::Lexer& lexer);
    static void entity_declaration(lexer::Lexer& lexer);
    static void generate_statement(lexer::Lexer& lexer);
    static void if_statement(lexer::Lexer& lexer);
    static void loop_statement(lexer::Lexer& lexer);
    static void package_body(lexer::Lexer& lexer);
    static void package_declaration(lexer::Lexer& lexer);
    static void physical_type_definition(lexer::Lexer& lexer);
    static void process_statement(lexer::Lexer& lexer);
    static void protected_type_body(lexer::Lexer& lexer);
    static void protected_type_declaration(lexer::Lexer& lexer);
    static void record_type_definition(lexer::Lexer& lexer);
    static void function_decl_body(lexer::Lexer& lexer);
    static void procedure_decl_body(lexer::Lexer& lexer);
    static void match_any(lexer::Lexer& lexer);
    static void temp_no_block_match(lexer::Lexer& lexer);
    static void identifier_mopt(lexer::Lexer& lexer);

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    using OptToken = tl::optional<lexer::TokenVariant>;

    template<typename ...T>
    static void check_eof(const tl::optional<T>&... opts)
    {
        if ((... || (! opts))) {
            throw "Unexpected end of input!";
        }
    }

    static void temp_match(lexer::Lexer& lexer)
    {
        const auto& tok0 { lexer.peek()[0] };
        const auto& tok1 { lexer.peek()[1] };
        const auto& tok2 { lexer.peek()[2] };

        if (tok0.has_value()) {
            std::visit(overloaded{
                [&](const lexer::Identifier& id) {
                    lexer.pop();
                    return;
                },
                [&](const lexer::Delimiter& delim) {
                    switch (delim.kind) {
                        case lexer::delimiter_kind::semicolon:
                            break;

                        default:
                            throw "Unexpected delimiter kind!";
                            break;
                    }
                },
                [&](const lexer::ReservedWord& res) {
                    switch (res.kind) {
                        case lexer::reserved_word_kind::ARCHITECTURE:
                            return architecture_body(lexer);
                            break;

                        case lexer::reserved_word_kind::FOR:
                            // Check more lookahead tokens
                            if (tok1.has_value() && tok2.has_value()) {
                                return std::visit(overloaded{
                                    [&](const lexer::Identifier& l, const lexer::ReservedWord& r) {
                                        if (r.kind == lexer::reserved_word_kind::IN) {
                                            return loop_statement(lexer);
                                        } else {
                                            return block_or_component_configuration(lexer);
                                        }
                                    },
                                    [&](const auto& l, const auto& r) {
                                        return block_or_component_configuration(lexer);
                                    }
                                }, tok1.value(), tok2.value());
                            } else {
                                throw "Unexpected end of input!";
                            }
                            break;

                        case lexer::reserved_word_kind::BLOCK:
                            return block_statement(lexer);
                            break;

                        case lexer::reserved_word_kind::CASE:
                            return case_statement(lexer);
                            break;

                        case lexer::reserved_word_kind::COMPONENT:
                            return component_declaration(lexer);
                            break;

                        case lexer::reserved_word_kind::CONFIGURATION:
                            return configuration_declaration(lexer);
                            break;

                        case lexer::reserved_word_kind::ENTITY:
                            return entity_declaration(lexer);
                            break;

                        case lexer::reserved_word_kind::GENERATE:
                            return generate_statement(lexer);
                            break;

                        case lexer::reserved_word_kind::IF:
                            return if_statement(lexer);
                            break;

                        case lexer::reserved_word_kind::LOOP:
                            loop_statement(lexer);
                            break;

                        case lexer::reserved_word_kind::PACKAGE:
                            // Check more lookahead tokens
                            if (tok1.has_value()) {
                                if (std::holds_alternative<lexer::ReservedWord>(tok1.value())
                                    && std::get<lexer::ReservedWord>(tok1.value()).kind == lexer::reserved_word_kind::BODY
                                ) {
                                    return package_body(lexer);
                                } else {
                                    return package_declaration(lexer);
                                }
                            } else {
                                throw "Unexpected end of input!";
                            }
                            break;

                        case lexer::reserved_word_kind::UNITS:
                            return physical_type_definition(lexer);
                            break;

                        case lexer::reserved_word_kind::PROCESS:
                            return process_statement(lexer);
                            break;

                        case lexer::reserved_word_kind::PROTECTED:
                            // Check more lookahead tokens
                            if (tok1.has_value()) {
                                if (std::holds_alternative<lexer::ReservedWord>(tok1.value())
                                    && std::get<lexer::ReservedWord>(tok1.value()).kind == lexer::reserved_word_kind::BODY
                                ) {
                                    return protected_type_body(lexer);
                                } else {
                                    return protected_type_declaration(lexer);
                                }
                            } else {
                                throw "Unexpected end of input!";
                            }
                            break;

                        case lexer::reserved_word_kind::RECORD:
                            return record_type_definition(lexer);
                            break;

                        case lexer::reserved_word_kind::FUNCTION:
                            return function_decl_body(lexer);
                            break;

                        case lexer::reserved_word_kind::PROCEDURE:
                            return procedure_decl_body(lexer);
                            break;

                        default:
                            throw "Unexpected reserved word token!";
                            break;
                    }
                },
                [&](const auto& tok) {
                    throw "Unexpected token type!";
                }
            }, tok0.value());
        }

        return;
    }

    static void architecture_body(lexer::Lexer& lexer)
    {
        const OptToken tok_begin_architecture { lexer.pop() };
        const OptToken tok_begin_identifier { lexer.pop() };
        check_eof(tok_begin_architecture, tok_begin_identifier);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::Identifier& r) {
                if (l.kind != lexer::reserved_word_kind::ARCHITECTURE) {
                    throw "Unexpected reserved word kind!";
                }
                // TODO: Process identifier
            },
            [](const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_architecture.value(), tok_begin_identifier.value());

        match_any(lexer);

        const OptToken tok_begin { lexer.pop() };
        check_eof(tok_begin);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::BEGIN) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin.value());

        match_any(lexer);

        const OptToken tok_end { lexer.pop() };
        check_eof(tok_end);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value());

        OptToken tok { lexer.pop() };
        check_eof(tok);

        std::visit(overloaded{
            [&](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::ARCHITECTURE) {
                    throw "Unexpected reserved word kind!";
                }
                tok = lexer.pop();
                check_eof(tok);
            },
            [](const auto&) { }
        }, tok.value());

        std::visit(overloaded{
            [&](const lexer::Identifier& l) {
                // TODO: Process identifier?
                tok = lexer.pop();
                check_eof(tok);
            },
            // Optional
            [](const auto&) { }
        }, tok.value());

        std::visit(overloaded{
            [](const lexer::Delimiter& l) {
                if (l.kind != lexer::delimiter_kind::semicolon) {
                    throw "Unexpected delimiter kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        return;
    }

    static void block_or_component_configuration(lexer::Lexer& lexer)
    {
        const OptToken tok_begin_for { lexer.pop() };
        check_eof(tok_begin_for);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::FOR) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_for.value());

        match_any(lexer);

        const OptToken tok_end { lexer.pop() };
        const OptToken tok_for { lexer.pop() };
        const OptToken tok_semicolon { lexer.pop() };
        check_eof(tok_end, tok_for);

        std::visit(overloaded{
            [](
                const lexer::ReservedWord& l,
                const lexer::ReservedWord& r,
                const lexer::Delimiter& semi
            ) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
                if (r.kind != lexer::reserved_word_kind::FOR) {
                    throw "Unexpected reserved word kind!";
                }
                if (semi.kind != lexer::delimiter_kind::semicolon) {
                    throw "Unexpected delimiter kind!";
                }
            },
            [](const auto&, const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value(), tok_for.value(), tok_semicolon.value());

        return;
    }

    static void block_statement(lexer::Lexer& lexer)
    {
        const OptToken tok_begin_block { lexer.pop() };
        check_eof(tok_begin_block);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::BLOCK) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_block.value());

        match_any(lexer);

        const OptToken tok_begin { lexer.pop() };
        check_eof(tok_begin);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::BEGIN) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin.value());

        match_any(lexer);

        const OptToken tok_end { lexer.pop() };
        const OptToken tok_block { lexer.pop() };
        check_eof(tok_end, tok_block);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
                if (r.kind != lexer::reserved_word_kind::BLOCK) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value(), tok_block.value());

        return;
    }

    static void case_statement(lexer::Lexer& lexer)
    {
        const OptToken tok_begin_case { lexer.pop() };
        check_eof(tok_begin_case);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::CASE) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_case.value());

        match_any(lexer);

        const OptToken tok_end { lexer.pop() };
        const OptToken tok_case { lexer.pop() };
        check_eof(tok_end, tok_case);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
                if (r.kind != lexer::reserved_word_kind::CASE) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value(), tok_case.value());

        return;
    }

    static void component_declaration(lexer::Lexer& lexer)
    {
        const OptToken tok_begin_component { lexer.pop() };
        const OptToken tok_begin_identifier { lexer.pop() };
        check_eof(tok_begin_component, tok_begin_identifier);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::Identifier& r) {
                if (l.kind != lexer::reserved_word_kind::COMPONENT) {
                    throw "Unexpected reserved word kind!";
                }
                // TODO: Process identifier?
            },
            [](const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_component.value(), tok_begin_identifier.value());

        match_any(lexer);

        const OptToken tok_end { lexer.pop() };
        const OptToken tok_component { lexer.pop() };
        check_eof(tok_end, tok_component);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
                if (r.kind != lexer::reserved_word_kind::COMPONENT) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value(), tok_component.value());

        return;
    }

    static void configuration_declaration(lexer::Lexer& lexer)
    {
        const OptToken tok_begin_configuration { lexer.pop() };
        const OptToken tok_begin_identifier { lexer.pop() };
        check_eof(tok_begin_configuration, tok_begin_identifier);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::Identifier& r) {
                if (l.kind != lexer::reserved_word_kind::CONFIGURATION) {
                    throw "Unexpected reserved word kind!";
                }
                // TODO: Process identifier!
            },
            [](const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_configuration.value(), tok_begin_identifier.value());

        match_any(lexer);

        const OptToken tok_end { lexer.pop() };
        check_eof(tok_end);

        // Handle "END"
        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value());

        OptToken tok { lexer.pop() };
        check_eof(tok);

        // Handle "CONFIGURATION"
        std::visit(overloaded{
            [&](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::CONFIGURATION) {
                    throw "Unexpected reserved word kind!";
                }

                tok = lexer.pop();
                check_eof(tok);
            },
            // "CONFIGURATION" reserved word is optional
            [](const auto&) {}
        }, tok.value());

        // Handle "identifier_opt"
        std::visit(overloaded{
            [&](const lexer::Identifier& l) {
                // TODO: Handle identifier?
                tok = lexer.pop();
                check_eof(tok);
            },
            // "identifier_opt" is optional
            [](const auto&) {}
        }, tok.value());

        // Handle ';'
        std::visit(overloaded{
            [](const lexer::Delimiter& l) {
                if (l.kind != lexer::delimiter_kind::semicolon) {
                    throw "Unexpected delimiter kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        return;
    }

    static void entity_declaration(lexer::Lexer& lexer)
    {
        const tl::optional<lexer::TokenVariant> tok_begin_entity { lexer.pop() };
        const tl::optional<lexer::TokenVariant> tok_begin_identifier { lexer.pop() };
        check_eof(tok_begin_entity, tok_begin_identifier);

        // Handle "ENTITY identifier"
        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::Identifier& r) {
                if (l.kind != lexer::reserved_word_kind::ENTITY) {
                    throw "Unexpected reserved word kind";
                }
                // TODO: Process identifier
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_entity.value(), tok_begin_identifier.value());

        match_any(lexer);

        // Handle "BEGIN match_any"
        if (const auto& temp_tok = lexer.peek()[0];
            std::holds_alternative<lexer::ReservedWord>(temp_tok.value())
            && (
                std::get<lexer::ReservedWord>(temp_tok.value()).kind
                == lexer::reserved_word_kind::BEGIN
            )
        ) {
            // Discard the return value, as we checked it beforehand
            lexer.pop();
            match_any(lexer);
        }

        const tl::optional<lexer::TokenVariant> tok_end { lexer.pop() };
        check_eof(tok_end);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& others) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value());

        tl::optional<lexer::TokenVariant> tok { lexer.pop() };
        check_eof(tok);

        // Handle "ENTITY"
        std::visit(overloaded{
            [&](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::ENTITY) {
                    throw "Unexpected reserved word kind!";
                }
                tok = lexer.pop();
                check_eof(tok);
            },
            // "ENTITY" is optional
            [](const auto& others) { }
        }, tok.value());

        // Handle "identifier_opt"
        std::visit(overloaded{
            [&](const lexer::Identifier& l) {
                // TODO: Check identifier?
                tok = lexer.pop();
                check_eof(tok);
            },
            // "identifier_opt" is optional
            [](const auto& others) { }
        }, tok.value());

        // Handle ';'
        std::visit(overloaded{
            [&](const lexer::Delimiter& l) {
                if (l.kind != lexer::delimiter_kind::semicolon) {
                    throw "Unexpected delimiter kind!";
                }
            },
            [](const auto& others) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        return;
    }

    static void generate_statement(lexer::Lexer& lexer)
    {
        tl::optional<lexer::TokenVariant> tok_begin_generate { lexer.pop() };
        check_eof(tok_begin_generate);

        // Handle "GENERATE"
        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::GENERATE) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& l) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_generate.value());

        match_any(lexer);

        // Handle "BEGIN match_any"
        if (const auto& temp_tok = lexer.peek()[0];
            std::holds_alternative<lexer::ReservedWord>(temp_tok.value())
            && (
                std::get<lexer::ReservedWord>(temp_tok.value()).kind
                == lexer::reserved_word_kind::BEGIN
            )
        ) {
            // Discard the return value, as we checked it beforehand
            lexer.pop();
            match_any(lexer);
        }

        const tl::optional<lexer::TokenVariant> tok_end { lexer.pop() };
        const tl::optional<lexer::TokenVariant> tok_generate { lexer.pop() };
        check_eof(tok_end, tok_generate);

        // Handle "END GENERATE"
        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
                if (r.kind != lexer::reserved_word_kind::GENERATE) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value(), tok_generate.value());

        return;
    }

    static void if_statement(lexer::Lexer& lexer)
    {
        const tl::optional<lexer::TokenVariant> tok_begin_if { lexer.pop() };
        check_eof(tok_begin_if);

        // Match "IF"
        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::IF) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& l) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_if.value());

        match_any(lexer);

        const tl::optional<lexer::TokenVariant> tok_end { lexer.pop() };
        const tl::optional<lexer::TokenVariant> tok_if { lexer.pop() };
        check_eof(tok_end, tok_if);

        // Match "END IF"
        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
                if (r.kind != lexer::reserved_word_kind::IF) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value(), tok_if.value());

        return;
    }

    static void loop_statement(lexer::Lexer& lexer)
    {
        if (const auto& temp = lexer.peek()[0];
            std::holds_alternative<lexer::ReservedWord>(temp.value())
            && (
                std::get<lexer::ReservedWord>(temp.value()).kind
                == lexer::reserved_word_kind::FOR
            )
        ) {
            lexer.pop();

            const OptToken tok_identifier { lexer.pop() };
            const OptToken tok_in { lexer.pop() };
            check_eof(tok_identifier, tok_in);

            std::visit(overloaded{
                [](const lexer::Identifier& l, const lexer::ReservedWord& r) {
                    // TODO: Process identifier?
                    if (r.kind != lexer::reserved_word_kind::IN) {
                        throw "Unexpected reserved word kind!";
                    }
                },
                [](const auto&, const auto&) {
                    throw "Unexpected token kind!";
                }
            }, tok_identifier.value(), tok_in.value());

            temp_no_block_match(lexer);
        }

        const OptToken tok_begin_loop { lexer.pop() };
        check_eof(tok_begin_loop);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::BEGIN) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_loop.value());

        match_any(lexer);

        const OptToken tok_end { lexer.pop() };
        const OptToken tok_loop { lexer.pop() };
        check_eof(tok_end, tok_loop);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, lexer::ReservedWord& r) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
                if (r.kind != lexer::reserved_word_kind::LOOP) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value(), tok_loop.value());

        return;
    }

    static void package_body(lexer::Lexer& lexer)
    {
        const tl::optional<lexer::TokenVariant> tok_package { lexer.pop() };
        const tl::optional<lexer::TokenVariant> tok_body { lexer.pop() };
        check_eof(tok_package, tok_body);

        // Match "PACKAGE BODY"
        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                if (l.kind != lexer::reserved_word_kind::PACKAGE) {
                    throw "Unexpected reserved word kind!";
                }
                if (r.kind != lexer::reserved_word_kind::BODY) {
                }
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_package.value(), tok_body.value());

        match_any(lexer);

        tl::optional<lexer::TokenVariant> tok { lexer.pop() };
        check_eof(tok);

        // Match "END"
        std::visit(overloaded{
            [&](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& others) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        tok = lexer.pop();
        check_eof(tok);

        // Match "PACKAGE BODY"
        std::visit(overloaded{
            [&](const lexer::ReservedWord& l) {
                if (l.kind == lexer::reserved_word_kind::PACKAGE) {
                    tok = lexer.pop();
                    check_eof(tok);

                    std::visit(overloaded{
                        [&](const lexer::ReservedWord& l) {
                            if (l.kind != lexer::reserved_word_kind::BODY) {
                                throw "Unexpected reserved word kind!";
                            }
                            tok = lexer.pop();
                            check_eof(tok);
                        },
                        [](const auto& others) {
                            // "PACKAGE" keyword MUST be followed by a "BODY" keyword
                            throw "Unexpected token kind!";
                        }
                    }, tok.value());
                }
            },
            [](const auto& others) {
                // Package body reserved words are optional
                return;
            }
        }, tok.value());

        // Match "identifier_opt"
        std::visit(overloaded{
            [&](const lexer::Identifier& l) {
                // TODO: Check identifier?
                tok = lexer.pop();
                check_eof(tok);
            },
            [](const auto& others) {
                // Package body identifier label is optional
                return;
            }
        }, tok.value());

        // Match ';'
        std::visit(overloaded{
            [&](const lexer::Delimiter& l) {
                if (l.kind != lexer::delimiter_kind::semicolon) {
                    throw "Unexpected delimiter kind!";
                }
            },
            [](const auto& others) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        return;
    }

    static void package_declaration(lexer::Lexer& lexer)
    {
        const tl::optional<lexer::TokenVariant> tok_package { lexer.pop() };
        const tl::optional<lexer::TokenVariant> tok_identifier { lexer.pop() };
        check_eof(tok_package, tok_identifier);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::Identifier& r) {
                if (l.kind != lexer::reserved_word_kind::PACKAGE) {
                    throw "Unexpected reserved word kind!";
                }
                // TODO: Handle identifier
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_package.value(), tok_identifier.value());

        match_any(lexer);

        tl::optional<lexer::TokenVariant> tok { lexer.pop() };
        check_eof(tok);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& others) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        tok = lexer.pop();
        check_eof(tok);

        std::visit(overloaded{
            [&](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::PACKAGE) {
                    throw "Unexpected reserved word kind!";
                }
                tok = lexer.pop();
                check_eof(tok);
            },
            [](const auto& others) {
                // Package keyword is optional
                return;
            }
        }, tok.value());

        std::visit(overloaded{
            [&](const lexer::Identifier& l) {
                // TODO: Check identifier?
                tok = lexer.pop();
                check_eof(tok);
            },
            [](const auto& others) {
                // Identifier is optional
                return;
            }
        }, tok.value());

        std::visit(overloaded{
            [&](const lexer::Delimiter& l) {
                if (l.kind != lexer::delimiter_kind::semicolon) {
                    throw "Unexpected delimiter kind!";
                }
            },
            [](const auto& others) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        return;
    }

    static void physical_type_definition(lexer::Lexer& lexer)
    {
        const tl::optional<lexer::TokenVariant> tok_units { lexer.pop() };
        check_eof(tok_units);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::UNITS) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& l) {
                throw "Unexpected token kind!";
            }
        }, tok_units.value());

        match_any(lexer);

        {
            const tl::optional<lexer::TokenVariant> tok_end { lexer.pop() };
            const tl::optional<lexer::TokenVariant> tok_units { lexer.pop() };
            check_eof(tok_end, tok_units);

            std::visit(overloaded{
                [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                    if (l.kind != lexer::reserved_word_kind::END) {
                        throw "Unexpected reserved word!";
                    }
                    if (r.kind != lexer::reserved_word_kind::UNITS) {
                        throw "Unexpected reserved word!";
                    }
                },
                [](const auto& l, const auto& r) {
                    throw "Unexpected token kind!";
                }
            }, tok_end.value(), tok_units.value());
        }

        return;
    }

    static void process_statement(lexer::Lexer& lexer)
    {
        const tl::optional<lexer::TokenVariant> tok_process { lexer.pop() };
        check_eof(tok_process);

        std::visit(overloaded{
            [](const lexer::ReservedWord& val) {
                if (val.kind != lexer::reserved_word_kind::PROCESS) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& val) {
                throw "Unexpected token kind!";
            }
        }, tok_process.value());

        match_any(lexer);

        const tl::optional<lexer::TokenVariant> tok_begin { lexer.pop() };
        check_eof(tok_begin);

        std::visit(overloaded{
            [](const lexer::ReservedWord& val) {
                if (val.kind != lexer::reserved_word_kind::BEGIN) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& val) {
                throw "Unexpected token kind!";
            }
        }, tok_begin.value());

        match_any(lexer);

        {
            const tl::optional<lexer::TokenVariant> tok_end { lexer.pop() };
            const tl::optional<lexer::TokenVariant> tok_process { lexer.pop() };
            check_eof(tok_end, tok_process);

            std::visit(overloaded{
                [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                    if (l.kind != lexer::reserved_word_kind::END) {
                        throw "Unexpected reserved word!";
                    }
                    if (r.kind != lexer::reserved_word_kind::PROCESS) {
                        throw "Unexpected reserved word!";
                    }
                },
                [](const auto& l, const auto& r) {
                    throw "Unexpected token kind!";
                }
            }, tok_end.value(), tok_process.value());
        }

        return;
    }

    static void protected_type_body(lexer::Lexer& lexer)
    {
        {
            const tl::optional<lexer::TokenVariant> tok_protected { lexer.pop() };
            const tl::optional<lexer::TokenVariant> tok_body { lexer.pop() };
            check_eof(tok_protected, tok_body);

            std::visit(overloaded{
                [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                    if (l.kind != lexer::reserved_word_kind::PROTECTED) {
                        throw "Unexpected reserved word!";
                    }
                    if (r.kind != lexer::reserved_word_kind::BODY) {
                        throw "Unexpected reserved word!";
                    }
                },
                [](const auto& l, const auto& r) {
                    throw "Unexpected token kind!";
                }
            }, tok_protected.value(), tok_body.value());
        }

        match_any(lexer);

        {
            const tl::optional<lexer::TokenVariant> tok_end { lexer.pop() };
            const tl::optional<lexer::TokenVariant> tok_protected { lexer.pop() };
            const tl::optional<lexer::TokenVariant> tok_body { lexer.pop() };
            check_eof(tok_end, tok_protected, tok_body);

            std::visit(overloaded{
                [](
                    const lexer::ReservedWord& end,
                    const lexer::ReservedWord& prot,
                    const lexer::ReservedWord& body
                ) {
                    if (end.kind != lexer::reserved_word_kind::END) {
                        throw "Unexpected reserved word!";
                    }
                    if (prot.kind != lexer::reserved_word_kind::PROTECTED) {
                        throw "Unexpected reserved word!";
                    }
                    if (body.kind != lexer::reserved_word_kind::BODY) {
                        throw "Unexpected reserved word!";
                    }
                },
                [](const auto& end, const auto& prot, const auto& body) {
                    throw "Unexpected token kind!";
                }
            }, tok_end.value(), tok_protected.value(), tok_body.value());
        }

        return;
    }

    static void protected_type_declaration(lexer::Lexer& lexer)
    {
        const tl::optional<lexer::TokenVariant> tok { lexer.pop() };
        check_eof(tok);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::PROTECTED) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& other) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        match_any(lexer);

        const tl::optional<lexer::TokenVariant> tok_end { lexer.pop() };
        const tl::optional<lexer::TokenVariant> tok_protected { lexer.pop() };
        check_eof(tok_end, tok_protected);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word!";
                }
                if (r.kind != lexer::reserved_word_kind::PROTECTED) {
                    throw "Unexpected reserved word!";
                }
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value(), tok_protected.value());

        return;
    }


    static void record_type_definition(lexer::Lexer& lexer)
    {
        const tl::optional<lexer::TokenVariant> tok { lexer.pop() };           
        check_eof(tok);

        std::visit(overloaded{
            [](const lexer::ReservedWord& val) {
                if (val.kind != lexer::reserved_word_kind::RECORD) {
                    throw "Unexpected reserved word!";
                }
            },
            [](const auto& val) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        match_any(lexer);

        const tl::optional<lexer::TokenVariant> tok_end { lexer.pop() };
        const tl::optional<lexer::TokenVariant> tok_record { lexer.pop() };
        check_eof(tok_end, tok_record);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l, const lexer::ReservedWord& r) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word value!";
                }
                if (r.kind != lexer::reserved_word_kind::RECORD) {
                    throw "Unexpected reserved word value!";
                }
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value(), tok_record.value());

        return;
    }

    static void function_decl_body(lexer::Lexer& lexer)
    {
        const OptToken tok_begin_function { lexer.pop() };
        check_eof(tok_begin_function);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::FUNCTION) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_function.value());

        identifier_mopt(lexer);

        const OptToken tok_is_or_semicolon { lexer.pop() };
        check_eof(tok_is_or_semicolon);

        if (const auto* semi = std::get_if<lexer::Delimiter>(&tok_is_or_semicolon.value())) {
            if (semi->kind != lexer::delimiter_kind::semicolon) {
                throw "Unexpected delimiter kind!";
            }
            // This is a function declaration /wo body, end parsing
            return;
        } else if (const auto* rw = std::get_if<lexer::ReservedWord>(&tok_is_or_semicolon.value())) {
            if (rw->kind != lexer::reserved_word_kind::IS) {
                throw "Unexpected reserved word kind!";
            }
        } else {
            throw "Unexpected token kind!";
        }

        match_any(lexer);

        const OptToken tok_begin { lexer.pop() };
        check_eof(tok_begin);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::BEGIN) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin.value());

        match_any(lexer);

        const OptToken tok_end { lexer.pop() };
        check_eof(tok_end);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value());

        OptToken tok { lexer.pop() };
        check_eof(tok);

        std::visit(overloaded{
            [&](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::FUNCTION) {
                    throw "Unexpected reserved word kind";
                }
                tok = lexer.pop();
                check_eof(tok);
            },
            // "FUNCTION" reserved word is optional
            [](const auto&) { }
        }, tok.value());

        std::visit(overloaded{
            [&](const lexer::Identifier& l) {
                // TODO: Process identifier?
                tok = lexer.pop();
                check_eof(tok);
            },
            // "identifier_opt" is optional
            [](const auto&) {}
        }, tok.value());

        std::visit(overloaded{
            [](const lexer::Delimiter& l) {
                if (l.kind != lexer::delimiter_kind::semicolon) {
                    throw "Unexpected delimiter kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        return;
    }

    static void procedure_decl_body(lexer::Lexer& lexer)
    {
        const OptToken tok_begin_procedure { lexer.pop() };
        check_eof(tok_begin_procedure);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::PROCEDURE) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_procedure.value());

        identifier_mopt(lexer);

        const OptToken tok_is_or_semicolon { lexer.pop() };
        check_eof(tok_is_or_semicolon);

        if (const auto* semi = std::get_if<lexer::Delimiter>(&tok_is_or_semicolon.value())) {
            if (semi->kind != lexer::delimiter_kind::semicolon) {
                throw "Unexpected delimiter kind!";
            }
            // This is a function declaration /wo body, end parsing
            return;
        } else if (const auto* rw = std::get_if<lexer::ReservedWord>(&tok_is_or_semicolon.value())) {
            if (rw->kind != lexer::reserved_word_kind::IS) {
                throw "Unexpected reserved word kind!";
            }
        } else {
            throw "Unexpected token kind!";
        }

        match_any(lexer);

        const OptToken tok_begin { lexer.pop() };
        check_eof(tok_begin);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::BEGIN) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin.value());

        match_any(lexer);

        const OptToken tok_end { lexer.pop() };
        check_eof(tok_end);

        std::visit(overloaded{
            [](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::END) {
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_end.value());

        OptToken tok { lexer.pop() };
        check_eof(tok);

        std::visit(overloaded{
            [&](const lexer::ReservedWord& l) {
                if (l.kind != lexer::reserved_word_kind::PROCEDURE) {
                    throw "Unexpected reserved word kind";
                }
                tok = lexer.pop();
                check_eof(tok);
            },
            // "FUNCTION" reserved word is optional
            [](const auto&) { }
        }, tok.value());

        std::visit(overloaded{
            [&](const lexer::Identifier& l) {
                // TODO: Process identifier?
                tok = lexer.pop();
                check_eof(tok);
            },
            // "identifier_opt" is optional
            [](const auto&) {}
        }, tok.value());

        std::visit(overloaded{
            [](const lexer::Delimiter& l) {
                if (l.kind != lexer::delimiter_kind::semicolon) {
                    throw "Unexpected delimiter kind!";
                }
            },
            [](const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok.value());

        return;
    }

    static void temp_no_block_match(lexer::Lexer& lexer)
    {
        bool do_match { true };

        while (do_match) {
            const OptToken& tok { lexer.peek()[0] };
            check_eof(tok);

            std::visit(overloaded{
                [&](const lexer::Identifier& l) {
                    lexer.pop();
                },
                [&](const lexer::ReservedWord& l) {
                    switch (l.kind) {
                        case lexer::reserved_word_kind::IN:
                        case lexer::reserved_word_kind::IS:
                            lexer.pop();
                            break;

                        default:
                            do_match = false;
                            break;
                    }
                },
                // Rule can be empty
                [&](const auto&) {
                    do_match = false;
                }
            }, tok.value());
        }

        return;
    }

    static void identifier_mopt(lexer::Lexer& lexer)
    {
        bool do_match { true };

        while (do_match) {
            const OptToken& tok { lexer.peek()[0] };
            check_eof(tok);

            std::visit(overloaded{
                [&](const lexer::Identifier& l) {
                    lexer.pop();
                },
                [&](const auto&) {
                    do_match = false;
                }
            }, tok.value());
        }

        return;
    }

} // namespace pre_parse::parser

} // namespace pre_parse