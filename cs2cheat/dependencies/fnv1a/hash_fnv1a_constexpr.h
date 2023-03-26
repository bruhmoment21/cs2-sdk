#pragma once
#include <stdint.h>

// FNV1a c++11 constexpr compile time hash functions, 32 and 64 bit
// str should be a null terminated string literal, value should be left out 
// e.g hash_32_fnv1a_const("example")
// code license: public domain or equivalent
// post: https://notes.underscorediscovery.com/constexpr-fnv1a/

constexpr uint32_t val_32_const = 0x811c9dc5;
constexpr uint32_t prime_32_const = 0x1000193;

inline constexpr uint32_t hash_32_fnv1a_const(const char* const str, const uint32_t value = val_32_const) noexcept {
	return (str[0] == '\0') ? value : hash_32_fnv1a_const(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
}