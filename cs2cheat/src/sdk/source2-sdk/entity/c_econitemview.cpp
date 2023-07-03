#include "c_econitemview.hpp"

#include "../../memory/memory.hpp"

CEconItem* C_EconItemView::GetSOCData() {
    CCSPlayerInventory* pInventory = CCSPlayerInventory::GetInstance();
    if (!pInventory) return nullptr;

    return pInventory->GetSOCDataForItem(m_iItemID());
}
