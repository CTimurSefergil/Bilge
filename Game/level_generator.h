#pragma once

#include "defines.h"
#include "Core/Logger/logger.h"
#include "Core/Renderer/renderer.h"

enum TileType
{
    Wall,
    Ground,
    Player
};

typedef struct
{
    TileType type;
    int value;
} TileMap;

void read_and_create_level(TileMap tile_map[], u32 tile_count, u32 string_axis_length, u32 start_pos_x, u32 start_pos_y, u32 width, u32 height);