#pragma once

// Comment if you'd wish to disable logging.
#define SDK_ENABLE_LOGGING

#define SDK_LOG_PROLOGUE() CLogger::Log("[prologue] {}", std::source_location::current().function_name())
#define SDK_LOG_PTR(x) reinterpret_cast<void*>(x)

class CLogger {
   public:
    static CLogger& Get() {
        static CLogger inst;
        return inst;
    }

    template <typename... Args>
    static void Log(const char* fmt, const Args&... args) {
        Get().LogInternal(fmt, args...);
    }

    void Initialize();

   private:
    template <typename... Args>
    void LogInternal(const char* fmt, const Args&... args) {
#ifdef SDK_ENABLE_LOGGING
        LogStringInternal(std::vformat(fmt, std::make_format_args(args...)));
#endif
    }

    void LogStringInternal(const std::string& str);

    ~CLogger();

#ifdef _WIN32
    FILE* m_ConsoleStream;
#endif
};
