cmake_minimum_required(VERSION 3.20)

if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

project(scarletutils_2113 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)

# Note: Geometry Dash 2.113 is a 32-bit (x86) application.
# Make sure to configure CMake with: cmake -B build -A Win32
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(WARNING "Geometry Dash 2.113 is a 32-bit application. Be sure to configure with -A Win32 when targeting MSVC.")
endif()

include(FetchContent)

# 1. Fetch MinHook
FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG        v1.3.3
)
FetchContent_GetProperties(minhook)
if(NOT minhook_POPULATED)
    FetchContent_Populate(minhook)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(MINHOOK_HDE "${minhook_SOURCE_DIR}/src/hde/hde64.c")
else()
    set(MINHOOK_HDE "${minhook_SOURCE_DIR}/src/hde/hde32.c")
endif()

set(MINHOOK_SOURCES
    ${minhook_SOURCE_DIR}/src/buffer.c
    ${minhook_SOURCE_DIR}/src/hook.c
    ${minhook_SOURCE_DIR}/src/trampoline.c
    ${MINHOOK_HDE}
)

# 2. Fetch ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.90.4
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp
)

set(SOURCES
    src/dllmain.cpp
    src/globals.cpp
    src/config.cpp
    src/gui/menu.cpp
    src/hooks/hooks.cpp
    ${IMGUI_SOURCES}
    ${MINHOOK_SOURCES}
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/minhook
    ${minhook_SOURCE_DIR}/include
    ${minhook_SOURCE_DIR}/src
    ${minhook_SOURCE_DIR}/src/hde
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32
    gdi32
    user32
    kernel32
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "scarletutils_2113"
    PREFIX ""
)
