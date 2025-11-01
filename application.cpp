#include "application.h"
#include "logger.h"
#include "platform.h"
#include "renderer.h"

#include <string.h>

// Application state (private to this module)
static ApplicationState app_state;

// Forward declaration for game interface
extern b8 game_initialize();
extern b8 game_update(f64 delta_time);
extern void game_render();
extern void game_shutdown();

b8 application_create(ApplicationConfig config)
{
    // Initialize logging system first
    if (!initialize_logging())
    {
        BFATAL("Failed to initialize logging system");
        return FALSE;
    }

    BINFO("=== Application Starting ===");
    BINFO("Application: %s", config.name);

    // Initialize application state
    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;
    app_state.last_time = 0.0;
    app_state.renderer = NULL;

    // Initialize platform layer
    if (!platform_startup(&app_state.platform, config.name,
        config.start_pos_x, config.start_pos_y,
        config.start_width, config.start_height))
    {
        BFATAL("Failed to initialize platform layer");
        shutdown_logging();
        return FALSE;
    }

    // Get render state from platform
    platform_get_render_state(&app_state.renderer);

    // Initialize renderer
    if (!initialize_renderer(app_state.renderer))
    {
        BFATAL("Failed to initialize renderer");
        platform_shutdown(&app_state.platform);
        shutdown_logging();
        return FALSE;
    }

    // Initialize game layer
    if (!game_initialize())
    {
        BFATAL("Failed to initialize game layer");
        shutdown_renderer(app_state.renderer);
        platform_shutdown(&app_state.platform);
        shutdown_logging();
        return FALSE;
    }

    BINFO("Application initialized successfully");
    return TRUE;
}

b8 application_run()
{
    BINFO("Entering main application loop");

    while (app_state.is_running)
    {
        // Process platform messages
        if (!platform_pump_messages(&app_state.platform))
        {
            app_state.is_running = FALSE;
            break;
        }

        if (!app_state.is_suspended)
        {
            // Calculate delta time (simplified for now)
            f64 current_time = 0.0; // TODO: Get actual time from platform
            f64 delta_time = current_time - app_state.last_time;
            app_state.last_time = current_time;

            // Update game logic
            if (!game_update(delta_time))
            {
                BFATAL("Game update failed");
                app_state.is_running = FALSE;
                break;
            }

            // Render frame
            game_render();
        }
    }

    BINFO("=== Application Shutting Down ===");

    game_shutdown();
    shutdown_renderer(app_state.renderer);
    platform_shutdown(&app_state.platform);
    shutdown_logging();

    return TRUE;
}

void application_get_render_state(RenderState** out_render_state)
{
    *out_render_state = app_state.renderer;
}

void application_request_shutdown()
{
    app_state.is_running = FALSE;
}

b8 application_is_running()
{
    return app_state.is_running;
}