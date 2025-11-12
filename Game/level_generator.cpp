#include "level_generator.h"

void read_and_create_level(TileMap tile_map[], u32 tile_count, u32 string_axis_length, u32 start_pos_x, u32 start_pos_y, u32 width, u32 height)
{
    for(u32 i = 0; i < string_axis_length; i++)
    {
        switch (tile_map[i].type)
        {
        case Wall:
            BTRACE("Wall, %d\n", tile_map[i].value);
            break;
        case Ground:
            BTRACE("Ground, %d\n", tile_map[i].value);
            break;
        case Player:
        
        default:
            break;
        }
    }
}