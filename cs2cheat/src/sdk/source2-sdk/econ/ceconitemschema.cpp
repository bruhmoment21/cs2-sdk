#include "ceconitemschema.hpp"

#include "../../memory/memory.hpp"

bool CPaintKit::UsesLegacyModel() {
    if (!this || !memory::fnIsPaintKitUsingLegacyModel) return false;
    return memory::fnIsPaintKitUsingLegacyModel(this->sName);
}
