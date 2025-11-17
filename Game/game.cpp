#include "defines.h"
#include "Core/Logger/logger.h"
#include "Core/Application/application.h"
#include "Core/Renderer/renderer.h"
#include "game_interface.h"
#include "level_generator.h"

//TODO: Kendi INPUT sistemime gecis
#include <iostream>
#include <windows.h>

static f32 pos_x = 1000.0f;
static f32 pos_y = 400.0f;

TileMap tile_map[] = 
{
    Null, Null,   Null,   Null,   Null,   Null,   Null,
    Null, Ground, Ground, Ground, Ground, Ground, Null,
    Null, Ground, Wall,   Wall,   Wall,   Ground, Null,
    Null, Ground, Wall,   Player, Wall,   Ground, Null,
    Null, Ground, Wall,   Wall,   Wall,   Ground, Null,
    Null, Ground, Ground, Ground, Ground, Ground, Null,
    Null, Null,   Null,   Null,   Null,   Null,   Null,
};

/*
Oyuncunun pozisyonu sabit kalsa, 
çevresindeki sabit sayıda kare, 
oyuncunun girdisine göre kordinat 
değerlerini değiştirse...
*/

b8 game_initialize()
{
    BINFO("Game layer initialized successfully");

/*
    BFATAL("BFATAL");
    BERROR("BERROR");
    BWARN("BWARN");
    BDEBUG("BDEBUG");
    BINFO("BINFO");
    BTRACE("BTRACE");
*/
    
    u32 tile_count = sizeof(tile_map) / sizeof(tile_map[0]);
    TileMapCompleteRules rules = read_and_find_rules(tile_map, tile_count, 7, 0, 0, 0, 0);
    
    BDEBUG("Total tile types with rules: %u", (u32)rules.type_rules.size());
    
    for(u32 i = 0; i < rules.type_rules.size(); i++)
    {
        TileMapTypeRules& rule = rules.type_rules[i];
        const char* type_name = "";
        switch(rule.type)
        {
            case TileType::Wall: type_name = "Wall"; break;
            case TileType::Ground: type_name = "Ground"; break;
            case TileType::Player: type_name = "Player"; break;
            case TileType::Null: type_name = "Null"; break;
            default: type_name = "Unknown"; break;
        }
        
        BFATAL("Rules for %s:", type_name);
        
        if(rule.x_minus_neighbors.size() > 0)
        {
            BTRACE("  X-minus neighbors: ");
            for(auto& neighbor : rule.x_minus_neighbors)
            {
                const char* neighbor_name = "";
                switch(neighbor)
                {
                    case TileType::Wall: neighbor_name = "Wall"; break;
                    case TileType::Ground: neighbor_name = "Ground"; break;
                    case TileType::Player: neighbor_name = "Player"; break;
                    case TileType::Null: neighbor_name = "Null"; break;
                    default: neighbor_name = "Unknown"; break;
                }
                BDEBUG("    - %s", neighbor_name);
            }
        }
        
        if(rule.x_plus_neighbors.size() > 0)
        {
            BTRACE("  X-plus neighbors: ");
            for(auto& neighbor : rule.x_plus_neighbors)
            {
                const char* neighbor_name = "";
                switch(neighbor)
                {
                    case TileType::Wall: neighbor_name = "Wall"; break;
                    case TileType::Ground: neighbor_name = "Ground"; break;
                    case TileType::Player: neighbor_name = "Player"; break;
                    case TileType::Null: neighbor_name = "Null"; break;
                    default: neighbor_name = "Unknown"; break;
                }
                BDEBUG("    - %s", neighbor_name);
            }
        }
        
        if(rule.y_minus_neighbors.size() > 0)
        {
            BTRACE("  Y-minus neighbors: ");
            for(auto& neighbor : rule.y_minus_neighbors)
            {
                const char* neighbor_name = "";
                switch(neighbor)
                {
                    case TileType::Wall: neighbor_name = "Wall"; break;
                    case TileType::Ground: neighbor_name = "Ground"; break;
                    case TileType::Player: neighbor_name = "Player"; break;
                    case TileType::Null: neighbor_name = "Null"; break;
                    default: neighbor_name = "Unknown"; break;
                }
                BDEBUG("    - %s", neighbor_name);
            }
        }
        
        if(rule.y_plus_neighbors.size() > 0)
        {
            BTRACE("  Y-plus neighbors: ");
            for(auto& neighbor : rule.y_plus_neighbors)
            {
                const char* neighbor_name = "";
                switch(neighbor)
                {
                    case TileType::Wall: neighbor_name = "Wall"; break;
                    case TileType::Ground: neighbor_name = "Ground"; break;
                    case TileType::Player: neighbor_name = "Player"; break;
                    case TileType::Null: neighbor_name = "Null"; break;
                    default: neighbor_name = "Unknown"; break;
                }
                BDEBUG("    - %s", neighbor_name);
            }
        }
    }
    
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