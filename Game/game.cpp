#include "defines.h"
#include "Core/Logger/logger.h"
#include "Core/Application/application.h"
#include "Core/Renderer/renderer.h"
#include "game_interface.h"

//TODO: Kendi INPUT sistemime gecis
#include <iostream>
#include <windows.h>

static f32 pos_x = 1000.0f;
static f32 pos_y = 400.0f;


/*
String, Hashmap ... ??? - 
[
Duvar3, Duvar3, Duvar2,Duvar2,Yer0,Yer0,Yer1,
Duvar2, Duvar3, Duvar2,Duvar2,Yer0,Yer0,Yer1,
Yer0,   Yer0,   Yer0,  Yer0,  Yer1,Yer1,Yer1
]
*/
/*
Oyuncunun pozisyonu sabit kalsa, 
çevresindeki sabit sayıda kare, 
oyuncunun girdisine göre kordinat 
değerlerini değiştirse...
*/
b8 game_initialize()
{
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
    f32 speed = 2000.f;
    f32 movement = (f32)(delta_time * speed);

    //TODO: Kendi INPUT sistemime gecis
    if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
        pos_y += movement;
    }
    if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
        pos_x -= movement;
    }
    if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
        pos_y -= movement;
    }
    if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
        pos_x += movement;
    }

    return TRUE;
}

void game_render()
{
    render_background(0x12345);

    draw_rectangle(100, 100, 700, 700, 0x54267);
    draw_rectangle(500, 500, 100, 100, 0x313162);

    draw_rectangle((u32)pos_x, (u32)pos_y, 300, 250, 0x648563);
}

void game_shutdown()
{
    BINFO("Shutting down game layer");
}

static GameInterface s_game_interface = 
{
    game_initialize,
    game_update,
    game_shutdown,
    game_render
};

GameInterface* get_game_interface()
{
    return &s_game_interface;
}

int main()
{
    ApplicationConfig config = {};
    config.start_pos_x = 100;
    config.start_pos_y = 100;
    config.start_width = 1000;
    config.start_height = 800;
    config.name = "Simulation";
    config.game_interface = get_game_interface();

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