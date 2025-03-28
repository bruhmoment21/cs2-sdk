#pragma once

#include <pointer/pointer.hpp>

/*
 *   CSigScan usage: (Should be used with stb.hh, see signatures.h for SDK_SIG)
 *
 *      Can be passed multiple signatures, the first FOUND signature is FINAL.
 *      Third argument is a list that consists of a signature and procedure.
 *          Procedure: Lambda passed after the signature that tells what's
                        done to the pointer after being successfully found.
 *
 *       CSigScan variable(signatureName, libraryName, {
 *   1.      {SDK_SIG("8B D3 E8 ?")},
 *   2.      {SDK_SIG("8B D3 E8 ?"), [](CPointer& ptr) { ptr.Absolute(3, 0); }},
 *   3.      ...,
 *   4.      ...,
 *       });
 *
 *      See also CPointer usage for GetPtr() and GetPtrAs().
 */
class CSigScan {
   public:
    using ProcedureFn = std::function<void(CPointer&)>;

    struct SigData_t {
        std::span<const int> m_Signature;
        ProcedureFn m_Procedure;
    };

    CSigScan(const char* signatureName, const char* libraryName, const std::initializer_list<SigData_t>& data);

    auto GetPtr() const { return m_Value; }

    template <typename T>
    auto GetPtrAs() const {
        return m_Value.Get<T>();
    }

    CSigScan(const CSigScan&) = delete;
    CSigScan& operator=(const CSigScan&) = delete;

   private:
    friend class CSigScanManager;

    void FindSignature();
    auto FreeData() { std::vector<SigData_t>().swap(m_Data); }

    const char* m_Name;
    const char* m_LibraryName;

    std::vector<SigData_t> m_Data;

    CPointer m_Value;
};

class CSigScanManager {
   public:
    static CSigScanManager& Get() {
        static CSigScanManager inst;
        return inst;
    }

    void ScheduleScan(CSigScan* sigScan);
    void ProcessScans();

   private:
    std::vector<CSigScan*> m_ScheduledScans;
};
