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
    u8 set;
    union
    {
        char character;
        u32 unsigned32;
        u64 unsigned64;
        i32 signed32;
        i64 signed64;
    } key, value;
} KeyPair;

typedef struct Map
{
    u32 max_size;
    MapType value_type, key_type;
    KeyPair* map_values;
} Map;

Map* CreateMap(MapType key_type, MapType value_type, u32 max_size);
void DestroyMap(Map* map);

#define AppendMapItem(map, key, value)                                         \
    __AppendMapItem(map, (void*)key, (void*)value)
void __AppendMapItem(Map* map, void* key, void* value);

#define GetMapItemValue(map, key_value)                                        \
    _Generic((map->key_type),                                                  \
        0: GetMapItemValueChar(map, (void*)key_value),                         \
        1: GetMapItemValueU32(map, (void*)key_value),                          \
        2: GetMapItemValueU64(map, (void*)key_value),                          \
        3: GetMapItemValueI32(map, (void*)key_value),                          \
        4: GetMapItemValueI64(map, (void*)key_value))

void* GetMapItemValueChar(Map* map, char key_value);
void* GetMapItemValueU32(Map* map, u32 key_value);
void* GetMapItemValueU64(Map* map, u64 key_value);
void* GetMapItemValueI32(Map* map, i32 key_value);
void* GetMapItemValueI64(Map* map, i64 key_value);

KeyPair CreateKeyPair(MapType key_type, MapType value_type, void* key,
                      void* value);

#endif // _RENAI_MAP_
