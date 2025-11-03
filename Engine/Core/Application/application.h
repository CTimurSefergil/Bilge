#pragma once

#include "..\..\defines.h"
#include "..\..\Platform\platform.h"

#include <chrono>

// TODO: FARKLI BIR SISTEM HALINE GETIRILEBILIR :TODO
struct ElapsedTime
{
    std::chrono::system_clock::time_point tp1;
    std::chrono::system_clock::time_point tp2;
};

struct ApplicationConfig
{
    i16 start_pos_x;
    i16 start_pos_y;
    i16 start_width;
    i16 start_height;
    const char* name;
};

struct ApplicationState
{
    b8 is_running;
    b8 is_suspended;
    PlatformState platform;
    f64 last_time;
};

b8 application_create(ApplicationConfig config);
b8 application_run();

void application_request_shutdown();
