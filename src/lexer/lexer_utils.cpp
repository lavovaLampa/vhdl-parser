#include "lexer_utils.hpp"

#include <cstdint>
#include <iostream>
#include <result/result.hpp>
#include <string>

namespace Lexer {

auto parse_int32_t(
    const std::string& str,
    int32_t base
) noexcept -> cpp::result<int32_t, ParseError>
{
    try {
        size_t parsed_char_count { 0 };
        int32_t result { std::stoi(str, &parsed_char_count, base) };

        // Check if whole string was parsed
        if (parsed_char_count != str.size()) {
            return cpp::failure(ParseError {
                parse_error_kind::invalid_argument,
                "Whole string wasn't parsed!" });
        }

        return result;
    } catch (const std::invalid_argument& e) {
        return cpp::failure(ParseError { parse_error_kind::invalid_argument });
    } catch (const std::out_of_range& e) {
        return cpp::failure(ParseError { parse_error_kind::out_of_range });
    }
}

std::ostream& operator<<(std::ostream& out, const parse_error_kind& kind)
{
    const std::string_view kind_str_map[] {
        "Invalid Argument",
        "Out Of Range"
    };

    out << kind_str_map[static_cast<size_t>(kind)];
    return out;
}

std::ostream& operator<<(std::ostream& out, const ParseError& err)
{
    out << "Parse Error Kind:"
        << err.kind
        << "\n"
        << "Reason: "
        << err.msg.value_or("")
        << "\n";

    return out;
}

} // namespace Lexer