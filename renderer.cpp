#include <windows.h>

#include "defines.h"
#include "renderer.h"
#include "logger.h"

static b8 rectangle_error_logged = 0;

b8 initialize_renderer(RenderState* render_state)
{
    if (!render_state)
    {
        BERROR("Invalid render state provided to renderer");
        return FALSE;
    }

    BINFO("Renderer initialized successfully");
    return TRUE;
}

void shutdown_renderer(RenderState* render_state)
{
    BINFO("Renderer shutdown");
    // Cleanup if needed
}

void render_background(RenderState render_state, int color)
{
    if (!render_state.buffer_memory || render_state.buffer_width == 0 || render_state.buffer_height == 0)
    {
        return; // No buffer to render to
    }

    u32* pixel = (u32*)render_state.buffer_memory;
    for (u32 y = 0; y < render_state.buffer_height; y++)
    {
        for (u32 x = 0; x < render_state.buffer_width; x++)
        {
            *pixel = color;
            pixel++;
        }
    }
}

void draw_rectangle(RenderState render_state, u32 pos_x, u32 pos_y, u32 width, u32 height, int color)
{
    if (!render_state.buffer_memory || render_state.buffer_width == 0 || render_state.buffer_height == 0)
    {
        return; // No buffer to render to
    }

    // CURRENT MAX = 1920x1057
    pos_x = (pos_x * render_state.buffer_width) / 1920;
    pos_y = (pos_y * render_state.buffer_height) / 1057;
    width = (width * render_state.buffer_width) / 1920;
    height = (height * render_state.buffer_height) / 1057;

    b8 has_error = 0;

    if (pos_x >= render_state.buffer_width || pos_y >= render_state.buffer_height)
    {
        if (!rectangle_error_logged)
        {
            BERROR("Rectangle positions exceed buffer bounds: pos_x=%d, pos_y=%d (buffer: %dx%d)",
                pos_x, pos_y, render_state.buffer_width, render_state.buffer_height);
            rectangle_error_logged = 1;
        }
        has_error = 1;
    }

    u32 end_x = pos_x + width;
    u32 end_y = pos_y + height;

    if (end_x > render_state.buffer_width || end_y > render_state.buffer_height)
    {
        if (!rectangle_error_logged)
        {
            BERROR("Rectangle dimensions exceed buffer bounds: end_x=%d, end_y=%d (buffer: %dx%d)",
                end_x, end_y, render_state.buffer_width, render_state.buffer_height);
            rectangle_error_logged = 1;
        }
        has_error = 1;
    }

    if (has_error)
    {
        return;
    }

    u32* base_pixel = (u32*)render_state.buffer_memory;

    for (u32 y = pos_y; y < end_y; y++)
    {
        for (u32 x = pos_x; x < end_x; x++)
        {
            // 1D to 2D array
            u32* pixel = base_pixel + (y * render_state.buffer_width + x);
            *pixel = color;
        }
    }
}

void reset_renderer_error_state()
{
    rectangle_error_logged = 0;
    BDEBUG("Renderer error state reset");
}