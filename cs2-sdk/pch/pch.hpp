#pragma once

#include <source_location>
#include <functional>
#include <optional>
#include <iostream>
#include <format>
#include <vector>
#include <thread>
#include <mutex>
#include <span>

#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#elif __linux__
#include <dlfcn.h>
#include <link.h>
#include <string.h>
#endif
