#pragma once

#include <sigscan/sigscan.hpp>

namespace signatures {
    extern CSigScan GetBaseEntity, GetHighestEntityIndex;
    extern CSigScan GetCSGOInput;
    extern CSigScan GetMatricesForView;
    extern CSigScan GetHitboxSet;
    extern CSigScan HitboxToWorldTransforms;
}  // namespace signatures
