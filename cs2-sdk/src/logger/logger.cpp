#include "pch.hpp"

#include <logger/logger.hpp>

void CLogger::Initialize() {
#if defined(SDK_ENABLE_LOGGING) && defined(_WIN32)
    AllocConsole();
    SetConsoleTitleA("cs2-sdk console");

    freopen_s(&m_ConsoleStream, "CONOUT$", "w", stdout);
#endif
}

void CLogger::LogStringInternal(const std::string& str) {
#ifdef SDK_ENABLE_LOGGING
    std::cout << "[cs2-sdk] " << str << '\n';
#endif
}

CLogger::~CLogger() {
#if defined(SDK_ENABLE_LOGGING) && defined(_WIN32)
    fclose(m_ConsoleStream);
    FreeConsole();
#endif
}
