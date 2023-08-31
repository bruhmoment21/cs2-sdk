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
    CEconItemSchema* pItemSchema =
        interfaces::pClient->GetEconItemSystem()->GetEconItemSchema();
    if (!pItemSchema) return;

    void* pAttributeDefinitionInterface =
        pItemSchema->GetAttributeDefinitionInterface(index);
    if (!pAttributeDefinitionInterface) return;

    uint8_t attributestringdata[0x100] = {};

    if (!memory::fnCAttributeStringInit) return;
    memory::fnCAttributeStringInit(attributestringdata, 0, 0);

    *reinterpret_cast<uint32_t*>(attributestringdata + 0x10) = 1;

    if (!memory::fnCAttributeStringFill) return;
    memory::fnCAttributeStringFill(
        *reinterpret_cast<void**>(attributestringdata + 0x18), value);

    if (!memory::fnSetDynamicAttributeValueString) return;
    memory::fnSetDynamicAttributeValueString(
        this, pAttributeDefinitionInterface, attributestringdata);
}

CEconItem* CEconItem::CreateInstance() {
    if (!memory::fnCreateSharedObjectSubclassEconItem) return nullptr;
    return memory::fnCreateSharedObjectSubclassEconItem();
}
