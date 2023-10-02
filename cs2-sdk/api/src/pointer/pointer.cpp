#include "pch.hpp"

#include <pointer/pointer.hpp>
#include <logger/logger.hpp>

CPointer CPointer::GetFieldPtr(int offset) const {
    if (IsValid()) {
        return m_Value + offset;
    }

    LogInvalid();

    static CPointer null{};
    return &null;
}

CPointer& CPointer::Offset(int offset) {
    if (IsValid()) {
        m_Value += offset;
    }

    return *this;
}

CPointer& CPointer::Absolute(int preAbs, int postAbs) {
    if (IsValid()) {
        Offset(preAbs);
        m_Value = m_Value + sizeof(int) + *reinterpret_cast<int*>(m_Value);
        Offset(postAbs);
    }

    return *this;
}

CPointer& CPointer::Dereference(int count) {
    if (IsValid()) {
        while (count-- != 0) {
            m_Value = *reinterpret_cast<uintptr_t*>(m_Value);
        }
    }

    return *this;
}

void CPointer::LogInvalid() const { CLogger::Log("[CPointer] Avoided performing an operation with nullptr. (Debug?)"); }
