#include "pch.hpp"

#include <sigscan/sigscan.hpp>
#include <logger/logger.hpp>

#include <memory/memory.hpp>

CSigScan::CSigScan(const char* name, const char* libraryName, const std::initializer_list<SigData_t>& data) {
#ifdef SDK_ENABLE_LOGGING
    m_Name = name;
#else
    m_Name = "";
#endif

    m_LibraryName = libraryName;

    m_Data.reserve(data.size());
    m_Data.insert(m_Data.end(), data.begin(), data.end());

    CMemory::ScheduleScan(this);
}

void CSigScan::FindSignature() {
    auto& library = CMemory::GetModule(m_LibraryName);
    if (!library) {
        return CLogger::Log("[signature] Couldn't find {} because {} was not loaded.", m_Name, m_LibraryName);
    }

    for (size_t i = 0; i < m_Data.size(); ++i) {
        const auto& data = m_Data[i];

        m_Value = library->FindPattern(data.m_Signature);
        if (m_Value.IsValid()) {
            if (data.m_Procedure) {
                data.m_Procedure(m_Value);
            }

            return CLogger::Log("[signature] {} -> {} [{}]", m_Name, m_Value.Get<void*>(), i);
        }
    }

    CLogger::Log("[signature] {} was not found!", m_Name);
}
