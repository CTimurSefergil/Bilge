cmake_minimum_required(VERSION 3.15)
project(Bilge VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Disable automatic reconfiguration on file changes
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Automatically collect all Engine source files
file(GLOB_RECURSE ENGINE_SOURCES
    "${CMAKE_SOURCE_DIR}/Engine/*.cpp"
)

# Automatically collect all Engine header files
file(GLOB_RECURSE ENGINE_HEADERS
    "${CMAKE_SOURCE_DIR}/Engine/*.h"
)

# Automatically collect all Game source files
file(GLOB_RECURSE GAME_SOURCES
    "${CMAKE_SOURCE_DIR}/Game/*.cpp"
)

# Automatically collect all Game header files
file(GLOB_RECURSE GAME_HEADERS
    "${CMAKE_SOURCE_DIR}/Game/*.h"
)

# Build Engine as DLL
add_library(BilgeEngine SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Engine compile definitions
target_compile_definitions(BilgeEngine PRIVATE BILGE_ENGINE_EXPORTS)

# Include Engine directory - all headers use relative paths from Engine/
target_include_directories(BilgeEngine PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine
)

# Link Windows libraries to Engine
if(WIN32)
    target_link_libraries(BilgeEngine PRIVATE user32 gdi32 kernel32)
endif()

# Build Game as executable
add_executable(BilgeGame ${GAME_SOURCES} ${GAME_HEADERS})

# Include directories for Game - all headers use relative paths
target_include_directories(BilgeGame PRIVATE
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/Game
)

# Link Game to Engine DLL
target_link_libraries(BilgeGame PRIVATE BilgeEngine)

# Compiler-specific options
if(MSVC)
    target_compile_options(BilgeEngine PRIVATE /W3)
    target_compile_options(BilgeGame PRIVATE /W3)
else()
    target_compile_options(BilgeEngine PRIVATE -Wall -Wextra)
    target_compile_options(BilgeGame PRIVATE -Wall -Wextra)
endif()

# Copy Engine DLL to Game directory
add_custom_command(TARGET BilgeEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:BilgeEngine>
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)