#include "c_econitemview.hpp"
#include "../../../interfaces/interfaces.hpp"
#include "../../../memory/memory.hpp"

const char* C_EconItemView::GetItemName(bool bUncustomized) {  // false =  +name tag
    return memory::fnGetItemName(this, bUncustomized);
}
