#pragma once
#include "defines.h"
#include <windows.h>

struct RenderState
{
    void* buffer_memory;
    u32 buffer_width, buffer_height;
    BITMAPINFO buffer_bitmap_info;
};

BILGE_API b8 initialize_renderer();
BILGE_API void shutdown_renderer();

BILGE_API void render_background(int color);
BILGE_API void draw_rectangle(u32 pos_x, u32 pos_y, u32 width, u32 height, int color);
BILGE_API void reset_renderer_error_state();