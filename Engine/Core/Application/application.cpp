#include "Core/Application/application.h"
#include "Core/Logger/logger.h"
#include "Platform/platform.h"
#include "Core/Renderer/renderer.h"

#include <string.h>

static ApplicationState app_state;

// TODO: FARKLI BIR SISTEM HALINE GETIRILEBILIR :TODO
ElapsedTime elapsed_time;

b8 application_create(ApplicationConfig config)
{
    if (!config.game_interface)
    {
        BFATAL("Game interface is null");
        return FALSE;
    }

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
    app_state.game_interface = config.game_interface;

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

    if (!app_state.game_interface->initialize())
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

            if (!app_state.game_interface->update(dt))
            {
                BFATAL("Game update failed");
                app_state.is_running = FALSE;
                break;
            }

            app_state.game_interface->render();
        }
    }

    BINFO("=== Application Shutting Down ===");

    app_state.game_interface->shutdown();
    shutdown_renderer();
    platform_shutdown(&app_state.platform);
    shutdown_logging();

    return TRUE;
}

void application_request_shutdown()
{
    app_state.is_running = FALSE;
}