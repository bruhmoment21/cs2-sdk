cmake_minimum_required(VERSION 3.23)
project(cs2-sdk)

# DEBUG or RELEASE
set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_C_FLAGS "-march=native --no-gnu-unique -DDISASM_DISTORM")
set(CMAKE_C_FLAGS_DEBUG "-g3")
set(CMAKE_C_FLAGS_RELEASE "-O3 -s -fvisibility=hidden -ffunction-sections -fdata-sections -flto -DNDEBUG")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fpermissive")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

# LIB_SOURCES refer to our source files, not dependencies such as imgui or funchook.
file(GLOB_RECURSE LIB_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/src/*.cpp")
file(GLOB_RECURSE SDK_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/sdk/src/*.cpp")
file(GLOB_RECURSE API_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/api/src/*.cpp")
file(GLOB_RECURSE FUNCHOOK_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/libs/funchook/src/*.c")
file(GLOB_RECURSE DISTORM_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/libs/distorm/src/*.c")
file(GLOB_RECURSE IMGUI_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/libs/imgui/*.cpp")

# Remove Windows specific source files.
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/src/hooks/input/wndproc/wndproc_hooks.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/src/hooks/render/directx11/directx11_hooks.cpp")

list(REMOVE_ITEM FUNCHOOK_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/libs/funchook/src/funchook_windows.c")

list(REMOVE_ITEM IMGUI_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/libs/imgui/imgui_impl_win32.cpp")
list(REMOVE_ITEM IMGUI_SOURCES "${CMAKE_SOURCE_DIR}/cs2-sdk/libs/imgui/imgui_impl_dx11.cpp")

# Enables warnings for our source files.
set_property(SOURCE ${LIB_SOURCES} ${SDK_SOURCES} ${API_SOURCES} PROPERTY COMPILE_FLAGS "-Wall")

# External libraries
find_package(SDL3 REQUIRED)
find_package(Vulkan REQUIRED)

add_library(${CMAKE_PROJECT_NAME} SHARED 
    ${LIB_SOURCES} ${SDK_SOURCES} ${API_SOURCES} 
    ${FUNCHOOK_SOURCES}
    ${DISTORM_SOURCES} 
    ${IMGUI_SOURCES}
)

target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SOURCE_DIR}/cs2-sdk/pch/pch.hpp>")
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -Wl,--no-undefined -Wl,--gc-sections)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${SDL3_LIBRARIES} ${Vulkan_LIBRARIES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/cs2-sdk/libs/funchook/include"
    "${CMAKE_SOURCE_DIR}/cs2-sdk/libs/distorm/include"
    "${CMAKE_SOURCE_DIR}/cs2-sdk/libs/distorm/src"
    "${CMAKE_SOURCE_DIR}/cs2-sdk/sdk/include"
    "${CMAKE_SOURCE_DIR}/cs2-sdk/api/include"
    "${CMAKE_SOURCE_DIR}/cs2-sdk/include"
    "${CMAKE_SOURCE_DIR}/cs2-sdk/libs"
    "${CMAKE_SOURCE_DIR}/cs2-sdk/pch"
)
