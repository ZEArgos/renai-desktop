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

void __AppendMapItem(Map* map, void* key, void* value)
{
    for (u32 index = 0; index < map->max_size; index++)
    {
        // if (map->map_values[index].set != FAILURE) continue;
        printf("e\n");
        map->map_values[index] =
            CreateKeyPair(map->key_type, map->value_type, key, value);
        return;
    }
}

#define TypeTransform(value_type, value)                                       \
    {                                                                          \
        (value_type == 0   ? *((char*)value)                                   \
         : value_type == 1 ? *((u32*)value)                                    \
         : value_type == 2 ? *((u64*)value)                                    \
         : value_type == 3 ? *((i32*)value)                                    \
                           : *((i64*)value))                                   \
    }

KeyPair CreateKeyPair(MapType key_type, MapType value_type, void* key,
                      void* value)
{
    return (struct KeyPair){SUCCESS, TypeTransform(key_type, key),
                            TypeTransform(value_type, value)};
}
