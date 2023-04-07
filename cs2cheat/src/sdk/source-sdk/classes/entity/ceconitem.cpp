#include "ceconitem.hpp"

#include "../../../memory/memory.hpp"

CEconItem* CEconItem::CreateInstance() {
    if (!memory::fnCreateSharedObjectSubclassEconItem) return nullptr;
    return memory::fnCreateSharedObjectSubclassEconItem();
}
