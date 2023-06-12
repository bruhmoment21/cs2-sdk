#include "ceconitem.hpp"

#include "../../interfaces/interfaces.hpp"
#include "../../memory/memory.hpp"

void CEconItem::SetDynamicAttributeValue(int index, void* value) {
    CEconItemSchema* pItemSchema =
        interfaces::pClient->GetEconItemSystem()->GetEconItemSchema();
    if (!pItemSchema) return;

    void* pAttributeDefinitionInterface =
        pItemSchema->GetAttributeDefinitionInterface(index);
    if (!pAttributeDefinitionInterface) return;

    if (!memory::fnSetDynamicAttributeValueUint) return;
    memory::fnSetDynamicAttributeValueUint(this, pAttributeDefinitionInterface,
                                           value);
}

void CEconItem::SetDynamicAttributeValueString(int index, const char* value) {
    // CS2FIXME: Function got inlined and cannot be sigscanned.
}

CEconItem* CEconItem::CreateInstance() {
    if (!memory::fnCreateSharedObjectSubclassEconItem) return nullptr;
    return memory::fnCreateSharedObjectSubclassEconItem();
}
