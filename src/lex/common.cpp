#include "common.hpp"

#include "token.hpp"

#include <cassert>
#include <gsl/assert>
#include <ranges>
#include <string_view>
#include <algorithm>

namespace lexer {

template <std::equality_comparable T, T M>
class filter_adjacent_pairs {
private:
    bool prev_matched { false };

public:
    auto operator()(T c) -> bool
    {
        const bool advance_write_ptr { (c != M) || (! this->prev_matched) };
        this->prev_matched = (c == M) && (! this->prev_matched);

        return advance_write_ptr;
    }
};

StringLiteral parse_string_literal(size_t offset, std::string_view view)
{
    Expects(view.size() >= 2);
    Expects(
        view[0] == '"'
        && view[view.size() - 1] == '"'
    );

    const char* const limit { view.data() + view.size() };
    const char* cursor { view.data() };
    const char* marker {};
    const char* ctxmarker {};

    auto string_view {
        std::string_view { view.begin() + 1, view.end() - 1 }
        | std::views::filter(filter_adjacent_pairs<char, '"'>())
    };

    StringLiteral result {
        { offset, view },
        std::string { string_view.begin(), string_view.end() },
        std::nullopt
    };

    /*!re2c
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

        operator_and        { result.operator_symbol = op_symbol::op_and; return result; }
        operator_or         { result.operator_symbol = op_symbol::op_or; return result; }
        operator_nand       { result.operator_symbol = op_symbol::nand; return result; }
        operator_nor        { result.operator_symbol = op_symbol::nor; return result; }
        operator_xor        { result.operator_symbol = op_symbol::op_xor; return result; }
        operator_xnor       { result.operator_symbol = op_symbol::xnor; return result; }
        operator_eq         { result.operator_symbol = op_symbol::eq; return result; }
        operator_neq        { result.operator_symbol = op_symbol::neq; return result; }
        operator_lt         { result.operator_symbol = op_symbol::lt; return result; }
        operator_lte        { result.operator_symbol = op_symbol::lte; return result; }
        operator_gt         { result.operator_symbol = op_symbol::gt; return result; }
        operator_gte        { result.operator_symbol = op_symbol::gte; return result; }
        operator_sll        { result.operator_symbol = op_symbol::sll; return result; }
        operator_srl        { result.operator_symbol = op_symbol::srl; return result; }
        operator_sla        { result.operator_symbol = op_symbol::sla; return result; }
        operator_sra        { result.operator_symbol = op_symbol::sra; return result; }
        operator_rol        { result.operator_symbol = op_symbol::rol; return result; }
        operator_ror        { result.operator_symbol = op_symbol::ror; return result; }
        operator_plus       { result.operator_symbol = op_symbol::plus; return result; }
        operator_minus      { result.operator_symbol = op_symbol::minus; return result; }
        operator_ampersand  { result.operator_symbol = op_symbol::ampersand; return result; }
        operator_mul        { result.operator_symbol = op_symbol::mul; return result; }
        operator_div        { result.operator_symbol = op_symbol::div; return result; }
        operator_mod        { result.operator_symbol = op_symbol::mod; return result; }
        operator_rem        { result.operator_symbol = op_symbol::rem; return result; }
        operator_exp        { result.operator_symbol = op_symbol::exp; return result; }
        operator_abs        { result.operator_symbol = op_symbol::abs; return result; }
        operator_not        { result.operator_symbol = op_symbol::op_not; return result; }

        * { return result; }
    */

    return result;
}

cpp::result<BasedLiteral, ParseError> parse_based_literal(
    size_t offset,
    std::string_view view) noexcept
{
    auto split_view {
        view
        | std::views::filter([](char cr) { return cr != '_'; })
        | std::views::transform([](unsigned char c) { return std::tolower(c); })
        | std::views::split('#')
        | std::views::transform([](auto view) {
              auto iter = view | std::views::common;
              return std::string { iter.begin(), iter.end() };
          })
    };

    auto split_view_iter { split_view.begin() };

    const std::string str_base { *split_view_iter };
    const std::string str_integer { *(++split_view_iter) };
    const std::string str_exponent { *(++split_view_iter) };

    // Based integer base part cannot be empty
    assert(! str_base.empty());
    // Based integer integer part cannot be empty
    assert(! str_integer.empty());

    auto integer_split {
        str_integer
        | std::views::split('.')
        | std::views::transform([](auto view) {
              auto iter = view | std::views::common;
              return std::string { iter.begin(), iter.end() };
          })
    };

    auto integer_split_iter { integer_split.begin() };

    const std::string str_decimal_part { *integer_split_iter };
    const std::string str_fractional_part { *(++integer_split_iter) };

    // Decimal part cannot be empty
    assert(! str_decimal_part.empty());

    int32_t base {};
    auto base_result { parse_int32_t(str_base) };
    if (base_result.has_error()) {
        return cpp::failure(ParseError {
            base_result.error().kind,
            "Unable to parse based integer range" });
    }
    base = base_result.value();
    if (! (2 <= base <= 16)) {
        return cpp::failure(ParseError {
            parse_error_kind::out_of_range,
            "Based integer base is out of range (must be between <2, 16>)" });
    }

    int32_t decimal_part {};
    auto decimal_result { parse_int32_t(str_decimal_part, base) };
    if (decimal_result.has_error()) {
        return cpp::failure(ParseError {
            decimal_result.error().kind,
            "Unable to parse integer part of the based integer" });
    }
    decimal_part = decimal_result.value();

    std::optional<int32_t> fractional_part { std::nullopt };
    if (! str_fractional_part.empty()) {
        auto fractional_result { parse_int32_t(str_fractional_part, base) };
        if (fractional_result.has_error()) {
            return cpp::failure(ParseError {
                fractional_result.error().kind,
                "Unable to parse fractional part of the based integer" });
        }
        fractional_part = fractional_result.value();
    }

    std::optional<int32_t> exponent { std::nullopt };
    if (! str_exponent.empty()) {
        std::string str_exponent_clean { str_exponent };

        if (str_exponent.starts_with('e')) {
            str_exponent_clean = str_exponent.substr(1);
        }

        auto exponent_result { parse_int32_t(str_exponent_clean, 10) };
        if (exponent_result.has_error()) {
            return cpp::failure(ParseError {
                exponent_result.error().kind,
                "Unable to parse exponent of the based integer" });
        }
        exponent = exponent_result.value();
    }

    return BasedLiteral {
        { offset, view },
        base,
        decimal_part,
        fractional_part,
        exponent
    };
}

cpp::result<CharacterLiteral, ParseError> parse_character_literal(
    size_t offset,
    std::string_view view) noexcept
{
    // TODO: Support UTF-8?
    char chr { view[1] };

    return CharacterLiteral {
        { offset, view },
        chr
    };
}

cpp::result<BitStringLiteral, ParseError> parse_bitstring_literal(
    size_t offset,
    std::string_view view) noexcept
{
    Expects(view.size() >= 3);
    Expects(view[1] == '"');
    Expects(view[view.size() - 1] == '"');
    Expects(
        std::tolower(static_cast<unsigned char>(view[0])) == 'b'
        || std::tolower(static_cast<unsigned char>(view[0])) == 'x'
        || std::tolower(static_cast<unsigned char>(view[0])) == 'o');

    auto filtered_view {
        view
        | std::views::filter([](char c) { return c != '_'; })
        | std::views::transform([](unsigned char c) { return std::tolower(c); })
        | std::views::common
    };

    // TODO: Why create intermediate string?
    std::string lower_string { filtered_view.begin(), filtered_view.end() };

    bit_string_base base { bit_string_base::binary };
    switch (lower_string[0]) {
        case 'x': base = bit_string_base::hexadecimal; break;
        case 'o': base = bit_string_base::octal; break;
        case 'b': base = bit_string_base::binary; break;
    }

    return BitStringLiteral {
        { offset, view },
        base,
        std::string { lower_string, 2, lower_string.size() - 3 }
    };
}

cpp::result<DecimalLiteral, ParseError> parse_decimal_literal(
    size_t offset,
    std::string_view view) noexcept
{
    Expects(view.size() >= 1);
    Expects(std::ranges::all_of(
        view,
        [](unsigned char c) {
            return (
                c == '_'
                || c == '.'
                || std::tolower(c) == 'e'
                || c == '+'
                || c == '-'
                || std::isdigit(c));
        }));

    auto filtered_view {
        view
        | std::views::filter([](char c) { return c != '_'; })
        | std::views::transform([](unsigned char c) { return std::tolower(c); })
        | std::views::split('e')
        | std::views::transform([](auto view) {
              auto iter = view | std::views::common;
              return std::string { iter.begin(), iter.end() };
          })
    };

    auto view_iter { filtered_view.begin() };

    const std::string str_integer { *view_iter };
    const std::string str_exponent { *(++view_iter) };

    auto integer_view {
        str_integer
        | std::views::split('.')
        | std::views::transform([](auto view) {
              auto iter = view | std::views::common;
              return std::string { iter.begin(), iter.end() };
          })
    };

    auto integer_iter { integer_view.begin() };

    const std::string str_decimal_part { *integer_iter };
    const std::string str_fraction_part { *(++integer_iter) };

    assert(! str_decimal_part.empty());

    int32_t decimal_part { 0 };
    auto decimal_result { parse_int32_t(str_decimal_part) };
    if (! decimal_result) {
        return cpp::failure(ParseError {
            decimal_result.error().kind,
            "Unable to parse decimal part of a decimal literal" });
    }
    decimal_part = decimal_result.value();

    std::optional<int32_t> fraction_part { std::nullopt };
    if (! str_fraction_part.empty()) {
        auto fraction_part_result { parse_int32_t(str_fraction_part) };
        if (fraction_part_result.has_error()) {
            return cpp::failure(ParseError {
                fraction_part_result.error().kind,
                "Unable to parse fractional part of a decimal literal" });
        }
        fraction_part = fraction_part_result.value();
    }

    std::optional<int32_t> exponent { std::nullopt };
    if (! str_exponent.empty()) {
        auto exponent_result { parse_int32_t(str_exponent) };
        if (exponent_result.has_error()) {
            return cpp::failure(ParseError {
                exponent_result.error().kind,
                "Unable to parse exponent of a decimal literal" });
        }
        exponent = exponent_result.value();
    }

    return DecimalLiteral {
        { offset, view },
        decimal_part,
        fraction_part,
        exponent
    };
}

BasicIdentifier parse_basic_identifier(
    size_t offset,
    std::string_view view)
{
    auto lower_view {
        view
        | std::views::transform([](unsigned char c) { return std::tolower(c); })
        | std::views::common
    };

    return BasicIdentifier {
        { lexeme_kind::basic_identifier, offset, view },
        std::string { lower_view.begin(), lower_view.end() }
    };
}

ExtendedIdentifier parse_extended_identifier(
    size_t offset,
    std::string_view view) noexcept
{
    Expects(view.size() >= 3);
    Expects(
        (view[0] == '\\')
        && (view[view.size() - 1] == '\\'));

    auto filter_view {
        std::string_view { view.begin() + 1, view.end() - 1 }
        | std::views::filter(filter_adjacent_pairs<char, '\\'>())
    };

    return ExtendedIdentifier {
        { offset, view },
        std::string { filter_view.begin(), filter_view.end() }
    };
}

Comment parse_comment(size_t offset, std::string_view view) noexcept
{
    Expects(view.size() >= 2);
    Expects(view.substr(0, 2) == "--");

    return Comment {
        { offset, view },
        // Skip start of comment
        std::string { view.substr(2) }
    };
}

cpp::result<int32_t, ParseError> parse_int32_t(
    const std::string& str,
    int32_t base) noexcept
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

} // namespace lexer