#pragma once

#define LOG(...) printf(__VA_ARGS__)

namespace console {
    void Initialize();
    void Shutdown();
}  // namespace console
