#include "Map.h"

Map* CreateMap(AmbiguousTypeSpecifier key_type,
               AmbiguousTypeSpecifier value_type, u32 max_size)
{
    Map* created_map = malloc(sizeof(Map));
    created_map->max_size = max_size;
    created_map->filled_size = 0;
    created_map->key_type = key_type;
    created_map->value_type = value_type;
    created_map->map_values = malloc(sizeof(KeyPair) * max_size);

    return created_map;
}

void DestroyMap(Map* map)
{
    free(map->map_values);
    free(map);
}

void __AppendMapItem(Map* map, void* key, void* value)
{
    for (u32 index = 0; index < map->max_size; index++)
    {
        if (map->map_values[index].set == true) continue;
        map->map_values[index] =
            CreateKeyPair(map->key_type, map->value_type, key, value);
        map->filled_size++;

        return;
    }
}

KeyPair CreateKeyPair(AmbiguousTypeSpecifier key_type,
                      AmbiguousTypeSpecifier value_type, void* key,
                      void* value)
{
    KeyPair created = {true};
    AssignAmbiguousType(&created.key, key_type, key);
    AssignAmbiguousType(&created.value, value_type, value);

    return created;
}

void* __GetMapItemValue(Map* map, void* key_value)
{
    for (u32 index = 0; index < map->max_size; index++)
    {
        if (!CompareAmbiguousType(&map->map_values[index].key,
                                  map->key_type, key_value))
            continue;

        return GetAmbiguousType(&map->map_values[index].value,
                                map->value_type);
    }
    return NULL;
}

void __RemoveMapItem(Map* map, void* key)
{
    for (u32 index = 0; index < map->max_size; index++)
    {
        if (!CompareAmbiguousType(&map->map_values[index].key,
                                  map->key_type, key))
            continue;

        for (u32 inner_index = index; inner_index < map->max_size - 1;
             inner_index++)
            map->map_values[inner_index] =
                map->map_values[inner_index + 1];
    }
}

KeyPair* __GetMapKeyPair(Map* map, void* key)
{
    for (u32 index = 0; index < map->max_size; index++)
    {
        if (!CompareAmbiguousType(&map->map_values[index].key,
                                  map->key_type, key))
            continue;

        return &map->map_values[index];
    }
    return NULL;
}

void ClearMap(Map* map)
{
    free(map->map_values);
    map->map_values = calloc(map->max_size, sizeof(KeyPair));
}
