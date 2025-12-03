#pragma once

#include "defines.h"
#include "Core/Logger/logger.h"
#include "Core/Renderer/renderer.h"

//TODO: Kendi Vector Veri Tipin
#include <vector>

enum TileType
{
    Wall,
    Ground,
    Player,
    Null,
    Count
};

typedef struct
{
    TileType type;
} TileMap;

typedef struct
{
    TileType type;
    std::vector<TileType> x_plus_neighbors;
    std::vector<TileType> x_minus_neighbors;
    std::vector<TileType> y_plus_neighbors;
    std::vector<TileType> y_minus_neighbors;
} TileMapTypeRules;

typedef struct
{
    std::vector<TileMapTypeRules> type_rules;
} TileMapCompleteRules;

TileMapCompleteRules read_and_find_rules(TileMap tile_map[], u32 tile_count, u32 string_axis_length);