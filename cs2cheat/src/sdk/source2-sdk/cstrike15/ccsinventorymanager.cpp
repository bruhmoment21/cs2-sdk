#include "ccsinventorymanager.hpp"

#include "../../memory/memory.hpp"

CCSInventoryManager* CCSInventoryManager::GetInstance() {
    if (!memory::fnGetInventoryManager) return nullptr;
    return memory::fnGetInventoryManager();
}
