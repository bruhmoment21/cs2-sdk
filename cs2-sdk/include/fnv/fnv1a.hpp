#pragma once

namespace fnv1a {
    constexpr uint32_t Hash(const char* str) noexcept { return (*str ? (Hash(str + 1) ^ *str) * 0x01000193 : 0x811c9dc5); }
}  // namespace fnv1a
