#include "level_generator.h"

TileMapCompleteRules read_and_find_rules(TileMap tile_map[], u32 tile_count, u32 string_axis_length, u32 start_pos_x, u32 start_pos_y, u32 width, u32 height)
{
    TileMapCompleteRules complete_rules;
    
    for(u32 i = 0; i < tile_count; i++)
    {
        TileType current_tile_type = tile_map[i].type;
        
        if(current_tile_type == TileType::Null)
        {
            continue;
        }
        
        TileMapTypeRules* current_rules = nullptr;
        for(auto& rule : complete_rules.type_rules)
        {
            if(rule.type == current_tile_type)
            {
                current_rules = &rule;
                break;
            }
        }
        
        if(!current_rules)
        {
            TileMapTypeRules new_rules;
            new_rules.type = current_tile_type;
            complete_rules.type_rules.push_back(new_rules);
            current_rules = &complete_rules.type_rules.back();
        }
        
        {
            TileType neighbor_type = tile_map[i - 1].type;
            if(neighbor_type != TileType::Null)
            {    
                bool found = false;
                for(auto& n : current_rules->x_minus_neighbors)
                {
                    if(n == neighbor_type)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    current_rules->x_minus_neighbors.push_back(neighbor_type);
                }
            }
        }
        
        {
            TileType neighbor_type = tile_map[i + 1].type;
            if(neighbor_type != TileType::Null)
            {
                bool found = false;
                for(auto& n : current_rules->x_plus_neighbors)
                {
                    if(n == neighbor_type)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    current_rules->x_plus_neighbors.push_back(neighbor_type);
                }
            }
        }
        
        {
            TileType neighbor_type = tile_map[i - string_axis_length].type;
            if(neighbor_type != TileType::Null)
            {
                bool found = false;
                for(auto& n : current_rules->y_minus_neighbors)
                {
                    if(n == neighbor_type)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    current_rules->y_minus_neighbors.push_back(neighbor_type);
                }
            }
        }
        
        {
            TileType neighbor_type = tile_map[i + string_axis_length].type;
            if(neighbor_type != TileType::Null)
            {
                bool found = false;
                for(auto& n : current_rules->y_plus_neighbors)
                {
                    if(n == neighbor_type)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    current_rules->y_plus_neighbors.push_back(neighbor_type);
                }
            }
        }
    }

    return complete_rules;
}