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

    if (!memory::fnSetDynamicAttributeValueFloat) return;
    memory::fnSetDynamicAttributeValueFloat(this, pAttributeDefinitionInterface,
                                            value);
}

void CEconItem::SetDynamicAttributeValueString(int index, const char* value) {
    CEconItemSchema* pItemSchema =
        interfaces::pClient->GetEconItemSystem()->GetEconItemSchema();
    if (!pItemSchema) return;

    void* pAttributeDefinitionInterface =
        pItemSchema->GetAttributeDefinitionInterface(index);
    if (!pAttributeDefinitionInterface) return;

    if (!memory::fnSetCustomNameOrDescAttribute) return;
    memory::fnSetCustomNameOrDescAttribute(this, pAttributeDefinitionInterface,
                                           value);
}

CEconItem* CEconItem::CreateInstance() {
    if (!memory::fnCreateSharedObjectSubclassEconItem) return nullptr;
    return memory::fnCreateSharedObjectSubclassEconItem();
}
