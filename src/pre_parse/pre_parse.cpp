#include "pre_parse.hpp"

#include "lex/lexer.hpp"
#include "lex/pre_parse_lexer.hpp"
#include "lex/token.hpp"

#include <variant>
#include <cassert>


namespace pre_parse {

struct ErrorDetails {
    std::string msg;
    size_t offset;
    size_t len;
};

struct DesignUnit {
    tl::optional<std::string> name;
    size_t offset;
    size_t len;
    library_unit_kind kind;
};

namespace parser {
    // Define a recursive descent parser rule
    #define RD_RULE(rule_name) \
        static void rule_name( \
            lexer::LexerBase& lexer, \
            int32_t level, \
            size_t& offset, \
            std::vector<DesignUnit>& units \
        )

    // Call a recursive descent parser rule
    #define CALL_RD_RULE(rule_name) \
        rule_name(lexer, level, offset, units)

    RD_RULE(architecture_body);
    RD_RULE(block_or_component_configuration);
    RD_RULE(block_statement);
    RD_RULE(case_statement);
    RD_RULE(component_declaration);
    RD_RULE(configuration_declaration);
    RD_RULE(entity_declaration);
    RD_RULE(generate_statement);
    RD_RULE(if_statement);
    RD_RULE(loop_statement);
    RD_RULE(package_body);
    RD_RULE(package_declaration);
    RD_RULE(physical_type_definition);
    RD_RULE(process_statement);
    RD_RULE(protected_type_body);
    RD_RULE(protected_type_declaration);
    RD_RULE(record_type_definition);
    RD_RULE(function_decl_body);
    RD_RULE(procedure_decl_body);
    RD_RULE(match_any);
    RD_RULE(temp_no_block_match);
    RD_RULE(identifier_mopt);

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

    static size_t get_new_offset(size_t offset, const lexer::TokenVariant& last_token)
    {
        return std::visit(overloaded{
            [&](const lexer::Token& l) {
                const size_t new_offset { l.offset + l.len };
                assert(new_offset >= offset);

                return new_offset;
            }
        }, last_token);
    }

    RD_RULE(match_any)
    {
        bool do_match { true };

        while (do_match) {
            const OptToken& tok { lexer.peek()[0] };
            const OptToken& tok_next { lexer.peek()[1] };

            if (tok) {
                std::visit(overloaded{
                    [&](const lexer::Identifier& l) {
                        // Consume and ignore
                        lexer.pop();
                    },
                    [&](const lexer::ReservedWord& l) {
                        switch (l.kind) {
                            case lexer::reserved_word_kind::IN:
                            case lexer::reserved_word_kind::IS:
                                // Consume & ignore
                                lexer.pop();
                                break;

                            case lexer::reserved_word_kind::ARCHITECTURE:
                                CALL_RD_RULE(architecture_body);
                                break;

                            case lexer::reserved_word_kind::BEGIN:
                                throw "Unexpected reserved word at this position!";
                                break;

                            case lexer::reserved_word_kind::BLOCK:
                                CALL_RD_RULE(block_statement);
                                break;

                            case lexer::reserved_word_kind::BODY:
                                throw "Unexpected reserved word at this position!";
                                break;

                            case lexer::reserved_word_kind::CASE:
                                CALL_RD_RULE(case_statement);
                                break;

                            case lexer::reserved_word_kind::COMPONENT:
                                CALL_RD_RULE(component_declaration);
                                break;

                            case lexer::reserved_word_kind::CONFIGURATION:
                                CALL_RD_RULE(configuration_declaration);
                                break;

                            case lexer::reserved_word_kind::END:
                                throw "Unexpected reserved word at this position!";
                                break;

                            case lexer::reserved_word_kind::ENTITY:
                                CALL_RD_RULE(entity_declaration);
                                break;

                            case lexer::reserved_word_kind::FOR:
                                // FIXME: Disambiguate with more lookahead
                                break;

                            case lexer::reserved_word_kind::FUNCTION:
                                CALL_RD_RULE(function_decl_body);
                                break;

                            case lexer::reserved_word_kind::GENERATE:
                                CALL_RD_RULE(generate_statement);
                                break;

                            case lexer::reserved_word_kind::IF:
                                CALL_RD_RULE(if_statement);
                                break;

                            case lexer::reserved_word_kind::LOOP:
                                CALL_RD_RULE(loop_statement);
                                break;

                            case lexer::reserved_word_kind::PACKAGE:
                                check_eof(tok_next);
                                if (std::holds_alternative<lexer::ReservedWord>(tok_next.value())
                                    && (
                                        std::get<lexer::ReservedWord>(tok_next.value()).kind
                                        == lexer::reserved_word_kind::BODY
                                    )
                                ) {
                                    CALL_RD_RULE(package_body);
                                } else {
                                    CALL_RD_RULE(package_declaration);
                                }
                                break;

                            case lexer::reserved_word_kind::PROCEDURE:
                                CALL_RD_RULE(procedure_decl_body);
                                break;

                            case lexer::reserved_word_kind::PROCESS:
                                CALL_RD_RULE(process_statement);
                                break;

                            case lexer::reserved_word_kind::PROTECTED:
                                check_eof(tok_next);
                                if (const auto* temp = std::get_if<lexer::ReservedWord>(&tok_next.value());
                                    temp->kind == lexer::reserved_word_kind::BODY
                                ) {
                                    CALL_RD_RULE(protected_type_body);
                                } else {
                                    CALL_RD_RULE(protected_type_declaration);
                                }
                                break;

                            case lexer::reserved_word_kind::RECORD:
                                CALL_RD_RULE(record_type_definition);
                                break;

                            case lexer::reserved_word_kind::UNITS:
                                CALL_RD_RULE(physical_type_definition);
                                break;

                            default:
                                throw "Unexpected token kind!";
                                break;
                        }
                    },
                    [&](const lexer::Delimiter& l) {
                        if (l.kind != lexer::delimiter_kind::semicolon) {
                            throw "Unexpected delimiter kind!";
                        }
                        // Consume & ignore
                        lexer.pop();
                    },
                    [](const auto&) {
                        throw "Unexpected token kind!";
                    }
                }, tok.value());
            } else {
                return;
            }
        }

        return;
    }

    RD_RULE(architecture_body)
    {
        const OptToken tok_begin_architecture { lexer.pop() };
        OptToken tok_begin_identifier { lexer.pop() };
        check_eof(tok_begin_architecture, tok_begin_identifier);

        std::string architecture_name { };

        std::visit(overloaded{
            [&](const lexer::ReservedWord& l, lexer::Identifier& r) {
                if (l.kind != lexer::reserved_word_kind::ARCHITECTURE) {
                    throw "Unexpected reserved word kind!";
                }
                architecture_name = std::move(r.val);
            },
            [](const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_architecture.value(), tok_begin_identifier.value());

        CALL_RD_RULE(match_any);

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

        CALL_RD_RULE(match_any);

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

        const size_t new_offset { get_new_offset(offset, tok.value()) };

        units.emplace_back(
            std::move(architecture_name),
            offset,
            static_cast<size_t>(new_offset - offset),
            library_unit_kind::architecture);

        offset = new_offset;

        return;
    }

    RD_RULE(block_or_component_configuration)
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

        CALL_RD_RULE(match_any);

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

    RD_RULE(block_statement)
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

        CALL_RD_RULE(match_any);

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

        CALL_RD_RULE(match_any);

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

    RD_RULE(case_statement)
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

        CALL_RD_RULE(match_any);

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

    RD_RULE(component_declaration)
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

        CALL_RD_RULE(match_any);

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

    RD_RULE(configuration_declaration)
    {
        const OptToken tok_begin_configuration { lexer.pop() };
        OptToken tok_begin_identifier { lexer.pop() };
        check_eof(tok_begin_configuration, tok_begin_identifier);

        std::string configuration_name { };

        std::visit(overloaded{
            [&](const lexer::ReservedWord& l, lexer::Identifier& r) {
                if (l.kind != lexer::reserved_word_kind::CONFIGURATION) {
                    throw "Unexpected reserved word kind!";
                }
                configuration_name = std::move(r.val);
            },
            [](const auto&, const auto&) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_configuration.value(), tok_begin_identifier.value());

        CALL_RD_RULE(match_any);

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

        const size_t new_offset { get_new_offset(offset, tok.value()) };

        units.emplace_back(
            configuration_name,
            offset,
            static_cast<size_t>(new_offset - offset),
            library_unit_kind::configuration
        );

        offset = new_offset;

        return;
    }

    RD_RULE(entity_declaration)
    {
        const tl::optional<lexer::TokenVariant> tok_begin_entity { lexer.pop() };
        tl::optional<lexer::TokenVariant> tok_begin_identifier { lexer.pop() };
        check_eof(tok_begin_entity, tok_begin_identifier);

        std::string entity_name { };

        // Handle "ENTITY identifier"
        std::visit(overloaded{
            [&](const lexer::ReservedWord& l, lexer::Identifier& r) {
                if (l.kind != lexer::reserved_word_kind::ENTITY) {
                    throw "Unexpected reserved word kind";
                }
                entity_name = std::move(r.val);
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_begin_entity.value(), tok_begin_identifier.value());

        CALL_RD_RULE(match_any);

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
            CALL_RD_RULE(match_any);
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

        const size_t new_offset { get_new_offset(offset, tok.value()) };

        units.emplace_back(
            std::move(entity_name),
            offset,
            static_cast<size_t>(new_offset - offset),
            library_unit_kind::entity);

        offset = new_offset;

        return;
    }

    RD_RULE(generate_statement)
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

        CALL_RD_RULE(match_any);

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
            CALL_RD_RULE(match_any);
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

    RD_RULE(if_statement)
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

        CALL_RD_RULE(match_any);

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

    RD_RULE(loop_statement)
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

            CALL_RD_RULE(temp_no_block_match);
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

        CALL_RD_RULE(match_any);

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

    RD_RULE(package_body)
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
                    throw "Unexpected reserved word kind!";
                }
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_package.value(), tok_body.value());

        CALL_RD_RULE(match_any);

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

        const size_t new_offset { get_new_offset(offset, tok.value()) };

        units.emplace_back(
            tl::nullopt,
            offset,
            static_cast<size_t>(new_offset - offset),
            library_unit_kind::package_body);

        offset = new_offset;

        return;
    }

    RD_RULE(package_declaration)
    {
        const tl::optional<lexer::TokenVariant> tok_package { lexer.pop() };
        tl::optional<lexer::TokenVariant> tok_identifier { lexer.pop() };
        check_eof(tok_package, tok_identifier);

        std::string package_name { };

        std::visit(overloaded{
            [&](const lexer::ReservedWord& l, lexer::Identifier& r) {
                if (l.kind != lexer::reserved_word_kind::PACKAGE) {
                    throw "Unexpected reserved word kind!";
                }
                package_name = std::move(r.val);
            },
            [](const auto& l, const auto& r) {
                throw "Unexpected token kind!";
            }
        }, tok_package.value(), tok_identifier.value());

        CALL_RD_RULE(match_any);

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

        const size_t new_offset { get_new_offset(offset, tok.value()) };

        units.emplace_back(
            std::move(package_name),
            offset,
            static_cast<size_t>(new_offset - offset),
            library_unit_kind::package);

        offset = new_offset;

        return;
    }

    RD_RULE(physical_type_definition)
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

        CALL_RD_RULE(match_any);

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

    RD_RULE(process_statement)
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

        CALL_RD_RULE(match_any);

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

        CALL_RD_RULE(match_any);

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

    RD_RULE(protected_type_body)
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

        CALL_RD_RULE(match_any);

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

    RD_RULE(protected_type_declaration)
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

        CALL_RD_RULE(match_any);

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


    RD_RULE(record_type_definition)
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

        CALL_RD_RULE(match_any);

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

    RD_RULE(function_decl_body)
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

        CALL_RD_RULE(identifier_mopt);

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

        CALL_RD_RULE(match_any);

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

        CALL_RD_RULE(match_any);

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

    RD_RULE(procedure_decl_body)
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

        CALL_RD_RULE(identifier_mopt);

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

        CALL_RD_RULE(match_any);

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

        CALL_RD_RULE(match_any);

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

    RD_RULE(temp_no_block_match)
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

    RD_RULE(identifier_mopt)
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

    #undef RD_RULE
    #undef CALL_RD_RULE

} // namespace pre_parse::parser

/**
 * @brief Pre-parse given VHDL files, discovering primary and secondary units.
 * 
 * @param file  Path to a VHDL source file.
 * @return std::vector<LibraryUnit> Vector of discovered library units.
 */
std::vector<LibraryUnit> pre_parse_file(fs::path file)
{
    std::vector<DesignUnit> units { };
    size_t offset { 0 };
    // FIXME: Open and read given file!
    lexer::PreParseLexer lexer { "omegalul" };

    parser::match_any(lexer, 0, offset, units);
}

} // namespace pre_parse