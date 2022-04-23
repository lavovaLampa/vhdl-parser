#include "parser/parser_defs.hpp"

#include "parser/vhdl_sem.tab.hh"

namespace parser {

void test()
{
    parser::Parser parser {};

    auto result { parser.parse() };

    if (result == 0) {
        std::cout << "Parsing SUCC" << std::endl;
    } else {
        std::cout << "Parsing BLOW" << std::endl;
    }
}
} // namespace parser