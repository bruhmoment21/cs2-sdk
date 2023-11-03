#pragma once

class FNV1A {
   public:
    // Not guaranteed to be hashed at compile-time.
    static constexpr uint32_t Hash(const char* string) {
        uint32_t hash = FNV_OFFSET_BASIS;
        for (size_t i = 0; string[i]; ++i) {
            hash = FNV_PRIME * (hash ^ static_cast<uint8_t>(string[i]));
        }

        return hash;
    }

    // Guaranteed to be hashed at compile-time.
    static consteval uint32_t HashConst(const char* string) {
        uint32_t hash = FNV_OFFSET_BASIS;
        for (size_t i = 0; string[i]; ++i) {
            hash = FNV_PRIME * (hash ^ static_cast<uint8_t>(string[i]));
        }

        return hash;
    }

   private:
    static constexpr uint32_t FNV_PRIME = 0x01000193, FNV_OFFSET_BASIS = 0x811c9dc5;
};
