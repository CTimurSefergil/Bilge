#include <windows.h>
#include <stdio.h>

#include "defines.h"
#include "Core/Renderer/renderer.h"
#include "Core/Logger/logger.h"
#include "Platform/platform.h"

// Win32 internal state
struct Win32InternalState
{
    HINSTANCE h_instance;
    HWND hwnd;
    HDC hdc;
    RenderState render_state;
    b8 should_quit;
};

static Win32InternalState* win32_state = NULL;

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param)
{
    if (!win32_state) return DefWindowProcW(hwnd, msg, w_param, l_param);

    switch (msg)
    {
    case WM_ERASEBKGND:
        // Notify the OS that erasing will be handled by the application to prevent flicker.
        return 1;
    case WM_CLOSE:
        BINFO("Window close requested");
        win32_state->should_quit = TRUE;
        return 0;
    case WM_DESTROY:
        BINFO("Window destroyed");
        return 0;
    case WM_SIZE:
    {
        RECT r;
        GetClientRect(hwnd, &r);
        u32 new_width = r.right - r.left;
        u32 new_height = r.bottom - r.top;

        RenderState* render_state = &win32_state->render_state;

        if (render_state->buffer_width != new_width || render_state->buffer_height != new_height)
        {
            BDEBUG("Buffer size changing from %dx%d to %dx%d",
                render_state->buffer_width, render_state->buffer_height,
                new_width, new_height);
        }

        render_state->buffer_width = new_width;
        render_state->buffer_height = new_height;

        u32 buffer_size = render_state->buffer_width * render_state->buffer_height * sizeof(u32);

        if (render_state->buffer_memory)
        {
            VirtualFree(render_state->buffer_memory, 0, MEM_RELEASE);
        }

        render_state->buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (!render_state->buffer_memory)
        {
            BFATAL("Failed to allocate buffer memory: %u bytes", buffer_size);
            win32_state->should_quit = TRUE;
            return 0;
        }

        render_state->buffer_bitmap_info.bmiHeader.biSize = sizeof(render_state->buffer_bitmap_info.bmiHeader);
        render_state->buffer_bitmap_info.bmiHeader.biWidth = render_state->buffer_width;
        render_state->buffer_bitmap_info.bmiHeader.biHeight = render_state->buffer_height;
        render_state->buffer_bitmap_info.bmiHeader.biPlanes = 1;
        render_state->buffer_bitmap_info.bmiHeader.biBitCount = 32;
        render_state->buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
        reset_renderer_error_state();

    } break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        // Key pressed/released
        //b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
        // TODO: Forward to input system
    } break;
    case WM_MOUSEMOVE:
    {
        // Mouse move
        //i32 x_position = GET_X_LPARAM(l_param);
        //i32 y_position = GET_Y_LPARAM(l_param);
        // TODO: Forward to input system
    } break;
    case WM_MOUSEWHEEL:
    {
        // TODO: Forward to input system
    } break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    {
        // TODO: Forward to input system
    } break;
    }

    return DefWindowProcW(hwnd, msg, w_param, l_param);
}

b8 platform_startup(PlatformState* platform_state, const char* application_name, i32 x, i32 y, i32 width, i32 height)
{
    // Allocate internal state
    win32_state = (Win32InternalState*)malloc(sizeof(Win32InternalState));
    if (!win32_state)
    {
        BFATAL("Failed to allocate Win32 internal state");
        return FALSE;
    }

    memset(win32_state, 0, sizeof(Win32InternalState));
    platform_state->internal_state = win32_state;

    win32_state->h_instance = GetModuleHandleW(0);
    win32_state->should_quit = FALSE;

    // Convert application name to wide string
    size_t name_len = strlen(application_name) + 1;
    wchar_t* wide_name = (wchar_t*)malloc(name_len * sizeof(wchar_t));
    mbstowcs_s(NULL, wide_name, name_len, application_name, name_len - 1);

    // Setup and register window class
    WNDCLASSW window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = L"GameWindowClass";
    window_class.lpfnWndProc = win32_process_message;
    window_class.hInstance = win32_state->h_instance;
    window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClassW(&window_class))
    {
        BFATAL("Failed to register window class");
        free(wide_name);
        return FALSE;
    }

    // Create window
    win32_state->hwnd = CreateWindowW(
        window_class.lpszClassName,
        wide_name,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        x, y, width, height,
        0, 0,
        win32_state->h_instance,
        0
    );

    free(wide_name);

    if (!win32_state->hwnd)
    {
        BFATAL("Failed to create window");
        return FALSE;
    }

    // Get device context
    win32_state->hdc = GetDC(win32_state->hwnd);

    BINFO("Win32 platform initialized successfully");
    return TRUE;
}

void platform_shutdown(PlatformState* platform_state)
{
    if (win32_state)
    {
        if (win32_state->render_state.buffer_memory)
        {
            VirtualFree(win32_state->render_state.buffer_memory, 0, MEM_RELEASE);
        }

        if (win32_state->hdc)
        {
            ReleaseDC(win32_state->hwnd, win32_state->hdc);
        }

        if (win32_state->hwnd)
        {
            DestroyWindow(win32_state->hwnd);
        }

        free(win32_state);
        win32_state = NULL;
    }

    platform_state->internal_state = NULL;
    BINFO("Win32 platform shut down");
}

b8 platform_pump_messages(PlatformState* platform_state)
{
    if (!win32_state) return FALSE;

    MSG message;
    while (PeekMessageW(&message, win32_state->hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    if (win32_state->render_state.buffer_memory)
    {
        StretchDIBits(
            win32_state->hdc,
            0, 0,
            win32_state->render_state.buffer_width,
            win32_state->render_state.buffer_height,
            0, 0,
            win32_state->render_state.buffer_width,
            win32_state->render_state.buffer_height,
            win32_state->render_state.buffer_memory,
            &win32_state->render_state.buffer_bitmap_info,
            DIB_RGB_COLORS,
            SRCCOPY
        );
    }

    return !win32_state->should_quit;
}

void platform_get_render_state(RenderState** out_render_state)
{
    if (win32_state)
    {
        *out_render_state = &win32_state->render_state;
    }
    else
    {
        *out_render_state = NULL;
    }
}