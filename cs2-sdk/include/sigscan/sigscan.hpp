#pragma once

#include <pointer/pointer.hpp>

class CSigScan {
   public:
    using ProcedureFn = std::function<void(CPointer&)>;

    struct SigData_t {
        std::span<const int> m_Signature;
        ProcedureFn m_Procedure;
    };

    CSigScan(const char* name, const char* libraryName, const std::initializer_list<SigData_t>& data);

    void FindSignature();

    auto GetPtr() const { return m_Value; }
    auto FreeData() { std::vector<SigData_t>().swap(m_Data); }

    template <typename T>
    auto GetPtrAs() const {
        return m_Value.Get<T>();
    }

    CSigScan(const CSigScan&) = delete;
    CSigScan& operator=(const CSigScan&) = delete;

   private:
    const char* m_Name;
    const char* m_LibraryName;

    std::vector<SigData_t> m_Data;

    CPointer m_Value;
};
