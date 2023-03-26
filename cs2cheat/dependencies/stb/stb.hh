/**
 * @file stb.hh
 * @author Cristei Gabriel-Marian (cristei.g772@gmail.com)
 * @brief Compile-time String To Bytes (STB)
 * @version 1.0
 * @date 2023-03-23
 * 
 * Last update: 03/23/2023 (mm/dd/yyyy): [Breaking update]
 * Modernize, undo some cancer, change some naming, file structure,
 * implement tests directly in file.
 *
 */

#ifndef STB_DEFINED
#define STB_DEFINED

#include <cstdint>
#include <array>

namespace stb {
namespace detail {
    // detail methods assume null terminator.

    template<std::size_t N>
    constexpr auto find_first_of_start(std::array<char, N> const& data, std::size_t start, char ch) noexcept {
        std::size_t idx = start;
        while (data[idx] != ch && idx < N)
            ++idx;

        return idx;
    }

    template<std::size_t N>
    constexpr auto find_first_not_of_start(std::array<char, N> const& data, std::size_t start, char ch) noexcept {
        if (start < N && data[start] != ch)
            return start;

        std::size_t idx = start;
        while (data[idx] == ch && idx < N)
            ++idx;

        return idx;
    }

    template<std::size_t N>
    constexpr auto find_last_of(std::array<char, N> const& data, char ch) noexcept {
        std::size_t idx = data.size() - 2;
        while (data[idx] != ch && idx >= 0)
            --idx;

        return idx;
    }

    template<std::size_t N>
    constexpr auto find_last_not_of(std::array<char, N> const& data, char ch) noexcept {
        std::size_t idx = data.size() - 2;
        while (data[idx] == ch && idx >= 0)
            --idx;

        return idx;
    }

    constexpr auto char_to_hex(char ch) noexcept {
        if (ch >= '0' && ch <= '9')
            return ch - '0';

        if (ch >= 'A' && ch <= 'F')
            return ch - 'A' + 10;

        return ch - 'a' + 10;
    }

    template<typename T, T F = 16>
    constexpr T concat_hex(T lhs, T rhs) noexcept {
        return F * lhs + rhs;
    }
}  // namespace detail

template<auto V>
struct consteval_value {
    constexpr static decltype(V) value = V;
};

template<std::size_t N>
struct fixed_string: public std::array<char, N + 1> {
    using std::array<char, N + 1>::array;

    constexpr fixed_string(const char* str) noexcept
        : std::array<char, N + 1>() {
        for (auto i = 0; i != N; ++i)
            (*this)[i] = str[i];
    }
};

template<std::size_t N>
fixed_string(const char (&)[N]) noexcept -> fixed_string<N - 1>;

template<char delimiter, char mask, typename element_type, element_type masked>
struct basic_hex_string_array_conversion {
    template<fixed_string str>
    struct build {
      private:
        struct parse {
            struct result {
                std::size_t delimiter_count;
                std::size_t start;
                std::size_t next;
                std::size_t end;
            };

            constexpr static auto get() noexcept {
                std::size_t count = 1;

                constexpr std::size_t start = detail::find_first_not_of_start(str, 0, delimiter);
                constexpr std::size_t next  = detail::find_first_of_start(str, start, delimiter);
                constexpr std::size_t end   = detail::find_last_not_of(str, delimiter);

                bool previous_delimiter = false;
                for (auto i = next; i < end; ++i) {
                    if (str[i] == delimiter) {
                        if (!previous_delimiter)
                            ++count;

                        previous_delimiter = true;
                    } else
                        previous_delimiter = false;
                }

                return result {
                    count,
                    start,
                    next,
                    end};
            }
        };

        constexpr static auto make() noexcept {
            constexpr auto data  = parse::get();
            constexpr auto count = data.delimiter_count;
            constexpr auto start = data.start;
            constexpr auto next  = data.next;
            constexpr auto end   = data.end;

            std::array<element_type, count> result = {};
            std::array<std::size_t, count> skips   = {};
            std::size_t skipped                    = 0;
            std::size_t traversed                  = start;

            bool previous_skip = false;
            for (auto i = start; i < end; ++i) {
                if (str[i] == delimiter) {
                    if (!previous_skip)
                        skips[skipped++] = traversed;

                    previous_skip = true;
                } else
                    previous_skip = false;

                ++traversed;
            }

            bool one_char = str[start + 1] == delimiter;
            result[0]     = static_cast<element_type>(str[start] == mask ? masked : (one_char ? detail::char_to_hex(str[start]) : detail::concat_hex(detail::char_to_hex(str[start]), detail::char_to_hex(str[start + 1]))));

            std::size_t conversions = 1;
            for (auto i = next; i < end; ++i) {
                for (auto entry : skips) {
                    if (entry == i && entry < end) {
                        std::size_t idx       = detail::find_first_not_of_start(str, i + 1, delimiter);
                        one_char              = str[idx + 1] == delimiter;
                        result[conversions++] = static_cast<element_type>(str[idx] == mask ? masked : (one_char ? detail::char_to_hex(str[idx]) : detail::concat_hex(detail::char_to_hex(str[idx]), detail::char_to_hex(str[idx + 1]))));
                    }
                }
            }

            return result;
        }

      public:
        constexpr static auto value = consteval_value<make()>::value;
    };
};

using hex_string_array_conversion = basic_hex_string_array_conversion<' ', '?', int, -1>;
using simple_conversion           = hex_string_array_conversion;
}  // namespace stb

#ifndef STB_OMIT_TESTS
struct _ignore_me_stb_compliance_tests {
    using conv_type = stb::simple_conversion;

    constexpr static auto value_1 = conv_type::build<"AA BB CC DD EE FF">::value;
    static_assert(value_1[0] == 0xAA);
    static_assert(value_1[1] == 0xBB);
    static_assert(value_1[2] == 0xCC);
    static_assert(value_1[3] == 0xDD);
    static_assert(value_1[4] == 0xEE);
    static_assert(value_1[5] == 0xFF);
    static_assert(value_1.size() == 6);

    constexpr static auto value_2 = conv_type::build<"        C 0f C    a B ef         ">::value;
    static_assert(value_2[0] == 0x0C);
    static_assert(value_2[1] == 0x0F);
    static_assert(value_2[2] == 0x0C);
    static_assert(value_2[3] == 0x0A);
    static_assert(value_2[4] == 0x0B);
    static_assert(value_2[5] == 0xEF);
    static_assert(value_2.size() == 6);

    constexpr static auto value_3 = conv_type::build<"AA bb CC dd         ">::value;
    static_assert(value_3[0] == 0xAA);
    static_assert(value_3[1] == 0xBB);
    static_assert(value_3[2] == 0xCC);
    static_assert(value_3[3] == 0xDD);
    static_assert(value_3.size() == 4);

    constexpr static auto value_4 = conv_type::build<"    aa bb ee       ff">::value;
    static_assert(value_4[0] == 0xAA);
    static_assert(value_4[1] == 0xBB);
    static_assert(value_4[2] == 0xEE);
    static_assert(value_4[3] == 0xFF);
    static_assert(value_4.size() == 4);
};
#endif

#endif
