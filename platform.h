#pragma once

#include "defines.h"

// Forward declarations
struct RenderState;

// Platform abstraction for core systems
struct PlatformState
{
    void* internal_state;
};

// Platform lifecycle
b8 platform_startup
(
    PlatformState* platform_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height
);

void platform_shutdown(PlatformState* platform_state);

b8 platform_pump_messages(PlatformState* platform_state);

// Platform utility functions
void platform_get_render_state(RenderState** out_render_state);