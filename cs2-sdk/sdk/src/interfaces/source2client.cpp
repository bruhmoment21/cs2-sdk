#include "pch.hpp"

#include <interfaces/source2client.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

CSource2Client* CSource2Client::Get() {
    static const auto inst = CMemory::GetInterface(CConstants::CLIENT_LIB, "Source2Client002");
    return inst.Get<CSource2Client*>();
}
