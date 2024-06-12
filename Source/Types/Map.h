/**
 * @file Map.h
 * @author Zenais Argos
 * @brief The implementation of my own map type in C.
 * @date 2024-06-10
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_MAP_
#define _RENAI_MAP_

#include <Declarations.h>

typedef enum MapType
{
    character,
    unsigned32,
    unsigned64,
    signed32,
    signed64
} MapType;

typedef struct KeyPair
{
    void *key, *value;
} KeyPair;

typedef struct Map
{
    u32 max_size;
    MapType value_type, key_type;
    KeyPair* map_values;
} Map;

Map* CreateMap(MapType key_type, MapType value_type, u32 max_size);
void DestroyMap(Map* map);

void AppendMapItem(Map* map, void* key, void* value);

#endif // _RENAI_MAP_
