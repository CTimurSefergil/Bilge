#pragma once
#include "..\..\defines.h"
#include <windows.h>

struct RenderState
{
    void* buffer_memory;
    u32 buffer_width, buffer_height;
    BITMAPINFO buffer_bitmap_info;
};

b8 initialize_renderer();
void shutdown_renderer();

void render_background(int color);
void draw_rectangle(u32 pos_x, u32 pos_y, u32 width, u32 height, int color);
void reset_renderer_error_state();