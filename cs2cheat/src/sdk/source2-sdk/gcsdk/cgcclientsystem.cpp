#include "cgcclientsystem.hpp"

#include "../../memory/memory.hpp"

CGCClientSystem* CGCClientSystem::GetInstance() {
    if (!memory::fnGetClientSystem) return nullptr;
    return memory::fnGetClientSystem();
}
