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

typedef struct KeyPair
{
    u8 set;
    AmbiguousType key, value;
} KeyPair;

typedef struct Map
{
    u32 max_size, filled_size;
    AmbiguousTypeSpecifier value_type, key_type;
    KeyPair* map_values;
} Map;

Map* CreateMap(AmbiguousTypeSpecifier key_type,
               AmbiguousTypeSpecifier value_type, u32 max_size);
void DestroyMap(Map* map);

#define AppendMapItem(map, key, value)                                         \
    __AppendMapItem(map, (void*)&key, (void*)&value)
void __AppendMapItem(Map* map, void* key, void* value);

#define GetMapItemValue(map, key_value)                                        \
    __GetMapItemValue(map, (void*)&key_value)
void* __GetMapItemValue(Map* map, void* key_value);

#define GetMapItemValueCHR(map, key) *((char*)GetMapItemValue(map, key))
#define GetMapItemValueU32(map, key) *((u32*)GetMapItemValue(map, key))
#define GetMapItemValueU64(map, key) *((u64*)GetMapItemValue(map, key))
#define GetMapItemValueI32(map, key) *((i32*)GetMapItemValue(map, key))
#define GetMapItemValueI64(map, key) *((i64*)GetMapItemValue(map, key))

KeyPair CreateKeyPair(AmbiguousTypeSpecifier key_type,
                      AmbiguousTypeSpecifier value_type, void* key,
                      void* value);

#endif // _RENAI_MAP_
