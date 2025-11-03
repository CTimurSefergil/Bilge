cmake_minimum_required(VERSION 3.15)
project(Bilge VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Engine source files
set(ENGINE_SOURCES
    Engine/Core/Application/application.cpp
    Engine/Core/Logger/logger.cpp
    Engine/Core/Renderer/renderer.cpp
    Engine/Platform/platform_win32.cpp
)

# Game source files
set(GAME_SOURCES
    Game/game.cpp
)

# Engine headers
set(ENGINE_HEADERS
    Engine/defines.h
    Engine/Core/Application/application.h
    Engine/Core/Application/game_interface.h
    Engine/Core/Logger/logger.h
    Engine/Core/Renderer/renderer.h
    Engine/Platform/platform.h
)

# Build Engine as DLL
add_library(BilgeEngine SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Engine compile definitions
target_compile_definitions(BilgeEngine PRIVATE BILGE_ENGINE_EXPORTS)

# Include directories for Engine
target_include_directories(BilgeEngine PRIVATE
    Engine
    Engine/Core/Logger
    Engine/Core/Application
    Engine/Core/Renderer
    Engine/Platform
)

# Link Windows libraries to Engine
if(WIN32)
    target_link_libraries(BilgeEngine PRIVATE user32 gdi32 kernel32)
endif()

# Build Game as executable
add_executable(BilgeGame ${GAME_SOURCES})

# Game needs to import from engine DLL
target_compile_definitions(BilgeGame PRIVATE BILGE_ENGINE_IMPORTS)

# Include directories for Game
target_include_directories(BilgeGame PRIVATE
    Engine
    Engine/Core/Logger
    Engine/Core/Application
    Engine/Core/Renderer
    Engine/Platform
    Game
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
if(WIN32)
    add_custom_command(TARGET BilgeGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:BilgeEngine>
        $<TARGET_FILE_DIR:BilgeGame>
    )
endif()