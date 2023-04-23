#include "../api/module/module.hpp"

#include "utils.hpp"

void utils::UnloadLibrary() {
    void CS2_UnloadLibrary();
    CS2_UnloadLibrary();
}

bool utils::IsUsingVulkan() {
    static const bool isUsingVulkan =
        CModule(RENDERSYSTEMVULKAN_DLL).IsLoaded();
    return isUsingVulkan;
}
