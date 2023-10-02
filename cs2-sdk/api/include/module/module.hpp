#pragma once

class CModule {
   public:
    CModule(const char* name);
    ~CModule();

    auto GetHash() const { return m_Hash; }
    auto GetHandle() const { return m_Handle; }
    auto GetName() const { return m_Name.c_str(); }

    void SetBounds(uintptr_t begin, uintptr_t size) {
        m_Begin = begin;
        m_Size = size;
    }

    // Returns false if module not loaded.
    bool Retrieve();

    uintptr_t GetInterface(const char* version);
    uintptr_t GetProcAddress(const char* procName);
    uintptr_t FindPattern(const std::span<const int>& pattern) const;

   private:
    void InitializeHandle();
    void InitializeBounds();

    std::string m_Name;
    uint32_t m_Hash;
    void* m_Handle;

    uintptr_t m_Begin, m_Size;
};
