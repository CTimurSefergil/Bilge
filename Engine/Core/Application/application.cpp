#include "application.h"
#include "..\Logger\logger.h"
#include "..\..\Platform\platform.h"
#include "..\Renderer\renderer.h"

#include <string.h>

// Application state (private to this module)
static ApplicationState app_state;

// Forward declaration for game interface
extern b8 game_initialize();
extern b8 game_update(f64 delta_time);
extern void game_render();
extern void game_shutdown();

// TODO: FARKLI BIR SISTEM HALINE GETIRILEBILIR :TODO
ElapsedTime elapsed_time;

b8 application_create(ApplicationConfig config)
{
    if (!initialize_logging())
    {
        BFATAL("Failed to initialize logging system");
        return FALSE;
    }

    BINFO("=== Application Starting ===");
    BINFO("Application: %s", config.name);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;
    app_state.last_time = 0.0;

    if (!platform_startup(&app_state.platform, config.name,
        config.start_pos_x, config.start_pos_y,
        config.start_width, config.start_height))
    {
        BFATAL("Failed to initialize platform layer");
        shutdown_logging();
        return FALSE;
    }

    if (!initialize_renderer())
    {
        BFATAL("Failed to initialize renderer");
        platform_shutdown(&app_state.platform);
        shutdown_logging();
        return FALSE;
    }

    if (!game_initialize())
    {
        BFATAL("Failed to initialize game layer");
        shutdown_renderer();
        platform_shutdown(&app_state.platform);
        shutdown_logging();
        return FALSE;
    }

    // TODO: FARKLI BIR SISTEM HALINE GETIRILEBILIR :TODO
    elapsed_time.tp1 = std::chrono::system_clock::now();
    elapsed_time.tp2 = std::chrono::system_clock::now();

    BINFO("Application initialized successfully");
    return TRUE;
}

b8 application_run()
{
    BINFO("Entering main application loop");

    while (app_state.is_running)
    {
        if (!platform_pump_messages(&app_state.platform))
        {
            app_state.is_running = FALSE;
            break;
        }

        if (!app_state.is_suspended)
        {
            // TODO: FARKLI BIR SISTEM HALINE GETIRILEBILIR :TODO
            elapsed_time.tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> delta_time = elapsed_time.tp2 - elapsed_time.tp1;
            elapsed_time.tp1 = elapsed_time.tp2;
            float dt = delta_time.count();

            if (!game_update(dt))
            {
                BFATAL("Game update failed");
                app_state.is_running = FALSE;
                break;
            }

            game_render();
        }
    }

    BINFO("=== Application Shutting Down ===");

    game_shutdown();
    shutdown_renderer();
    platform_shutdown(&app_state.platform);
    shutdown_logging();

    return TRUE;
}

void application_request_shutdown()
{
    app_state.is_running = FALSE;
}