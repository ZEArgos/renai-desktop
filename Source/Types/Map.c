#include "Map.h"

Map* CreateMap(MapType key_type, MapType value_type, u32 max_size)
{
    Map* created_map = malloc(sizeof(Map));
    created_map->max_size = max_size;
    created_map->key_type = key_type;
    created_map->value_type = value_type;
    created_map->map_values = malloc(sizeof(KeyPair) * max_size);

    return created_map;
}

void DestroyMap(Map* map)
{
    // cheeky little trick
    // https://stackoverflow.com/questions/11255869/free-delete-union-malloc-new-array-in-c-c
    free(map->map_values);
    free(map);
}

void CreateMapItem(Map* map, void* key, void* value) {}
