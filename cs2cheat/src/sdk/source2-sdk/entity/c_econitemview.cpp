#include "c_econitemview.hpp"

#include "../../memory/memory.hpp"

CEconItem* C_EconItemView::GetSOCData() {
    if (!memory::fnGetSOCData) return nullptr;
    return memory::fnGetSOCData(this);
}
