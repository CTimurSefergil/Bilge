cmake_minimum_required(VERSION 3.15)
project(Bilge VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    Engine
    Engine/Core/Logger
    Engine/Core/Application
    Engine/Core/Renderer
    Engine/Platform
    Game
)

# Source files
set(ENGINE_SOURCES
    Engine/Core/Application/application.cpp
    Engine/Core/Logger/logger.cpp
    Engine/Core/Renderer/renderer.cpp
    Engine/Platform/platform_win32.cpp
)

set(GAME_SOURCES
    Game/game.cpp
)

# Headers
set(ENGINE_HEADERS
    Engine/defines.h
    Engine/Core/Application/application.h
    Engine/Core/Logger/logger.h
    Engine/Core/Renderer/renderer.h
    Engine/Platform/platform.h
)

# Create executable
add_executable(Bilge ${ENGINE_SOURCES} ${GAME_SOURCES} ${ENGINE_HEADERS})

# Link Windows libraries
if(WIN32)
    target_link_libraries(Bilge user32 gdi32 kernel32)
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(Bilge PRIVATE /W3)
else()
    target_compile_options(Bilge PRIVATE -Wall -Wextra)
endif()

# Build commands
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/build")
execute_process(COMMAND ${CMAKE_COMMAND} --build . --config Release WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/build")