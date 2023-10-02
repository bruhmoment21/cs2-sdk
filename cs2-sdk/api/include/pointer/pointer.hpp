#pragma once

class CPointer {
   public:
    CPointer() { m_Value = 0; }

    template <typename T>
    CPointer(T value) {
        m_Value = (uintptr_t)(value);
    }

    bool IsValid() const { return m_Value != 0; }

    template <typename T>
    auto Get() const {
        return reinterpret_cast<T>(m_Value);
    }

    template <typename T>
    std::add_lvalue_reference_t<T> GetField(int offset) const {
        return *GetFieldPtr(offset).Get<T*>();
    }

    template <typename T, typename... Args>
    auto Call(Args... args) const {
        if constexpr (std::is_invocable_v<T, Args...>) {
            if (IsValid()) {
                return std::invoke(Get<T>(), std::forward<Args>(args)...);
            }
        } else {
            static_assert(std::is_invocable_v<T, Args...>, "T must be an invocable type.");
        }

        LogInvalid();
        return std::invoke_result_t<T, Args...>{};
    }

    CPointer GetFieldPtr(int offset) const;

    CPointer& Offset(int offset);
    CPointer& Absolute(int preAbs, int postAbs);
    CPointer& Dereference(int count);

   private:
    void LogInvalid() const;

    uintptr_t m_Value;
};
