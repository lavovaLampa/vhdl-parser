#include "pre_parse.hpp"

#include "lex/token.hpp"

#include <deque>

namespace pre_parse {

std::vector<LibraryUnit> pre_parse_file(fs::path file)
{
}

namespace recursive_descent {

    namespace parse {
        static void architecture_body(
            std::deque<lexer::Lexeme>& tokens,
            std::deque<LibraryUnit>& unit_list,
            std::deque<std::string>& error_list)
        {
        }

        static void block_or_component_configuration()
        {
        }

        static void block_statement()
        {
        }

        static void case_statement()
        {
        }

        static void component_declaration()
        {
        }

        static void configuration_declaration()
        {
        }

        static void entity_declaration()
        {
        }

        static void generate_statement()
        {
        }

        static void if_statement()
        {
        }

        static void loop_statement()
        {
        }

        static void package_body()
        {
        }

        static void package_declaration()
        {
        }

        static void physical_type_definition()
        {
        }

        static void process_statement()
        {
        }

        static void protected_type_body()
        {
        }

        static void record_type_definition()
        {
        }

        static void subprogram_body()
        {
        }

        static void block_list()
        {
        }

    } // namespace parse
} // namespace recursive_descent

} // namespace pre_parse