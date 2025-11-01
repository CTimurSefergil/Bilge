#include "defines.h"
#include "logger.h"
#include "application.h"
#include "renderer.h"

static RenderState* render_state = NULL;


// BURASI KOMPLE APPL?CATON TABAKASINA ATILAB?L?R
b8 game_initialize()
{
    BINFO("Initializing game layer");

    application_get_render_state(&render_state);

    if (!render_state)
    {
        BERROR("Failed to get render state from application");
        return FALSE;
    }

    BINFO("Game layer initialized successfully");
    BFATAL("BFATAL");
    BERROR("BERROR");
    BWARN("BWARN");
    BDEBUG("BDEBUG");
    BINFO("BINFO");
    BTRACE("BTRACE");
    return TRUE;
}

b8 game_update(f64 delta_time)
{
    return TRUE;
}

void game_render()
{
    if (!render_state)
    {
        BERROR("Render state is null, cannot render");
        return;
    }

    render_background(*render_state, 0x12345);

    draw_rectangle(*render_state, 100, 100, 700, 700, 0x54267);
    draw_rectangle(*render_state, 500, 500, 100, 100, 0x313162);
}

void game_shutdown()
{
    BINFO("Shutting down game layer");
    render_state = NULL;
}

int main()
{
    ApplicationConfig config = {};
    config.start_pos_x = 100;
    config.start_pos_y = 100;
    config.start_width = 1200;
    config.start_height = 800;
    config.name = "Game Engine";

    if (!application_create(config))
    {
        BFATAL("Failed to create application");
        return -1;
    }

    if (!application_run())
    {
        BFATAL("Application run failed");
        return -1;
    }

    return 0;
}