#pragma once

#include <cstdio>

#include "../../defines.hpp"

#ifdef CS2_SDK_ENABLE_LOGGING
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

namespace console {
    void Initialize();
    void Shutdown();
}  // namespace console
