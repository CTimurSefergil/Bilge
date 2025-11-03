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

BILGE_API void platform_shutdown(PlatformState* platform_state);

BILGE_API b8 platform_pump_messages(PlatformState* platform_state);

BILGE_API void platform_get_render_state(RenderState** out_render_state);
