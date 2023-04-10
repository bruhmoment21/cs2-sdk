#include "c_basemodelentity.hpp"

#include "../../memory/memory.hpp"

void C_BaseModelEntity::SetModel(const char* name) {
    if (!memory::fnSetModel) return;
    memory::fnSetModel(this, name);
}
