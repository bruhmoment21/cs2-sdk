#include "ceconitem.hpp"

#include "../../../interfaces/interfaces.hpp"
#include "../../../memory/memory.hpp"

void CEconItem::SetDynamicAttributeValue(int index, float value) {
    CEconItemSchema* pItemSchema =
        interfaces::pClient->GetEconItemSystem()->GetEconItemSchema();
    if (!pItemSchema) return;

    void* pAttributeDefinitionInterface =
        pItemSchema->GetAttributeDefinitionInterface(index);

    if (!pAttributeDefinitionInterface ||
        !memory::fnSetDynamicAttributeValueFloat)
        return;

    memory::fnSetDynamicAttributeValueFloat(this, pAttributeDefinitionInterface,
                                            &value);
}

CEconItem* CEconItem::CreateInstance() {
    if (!memory::fnCreateSharedObjectSubclassEconItem) return nullptr;
    return memory::fnCreateSharedObjectSubclassEconItem();
}
