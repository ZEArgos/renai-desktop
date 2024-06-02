/**
 * @file Texture.h
 * @author Zenais Argos
 * @brief Provides the logic and data structures needed to load/render textures
 * with needed size, color, and positioning.
 * @date 2024-06-01
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_TEXTURE_
#define _RENAI_TEXTURE_

#include "Declarations.h"

typedef struct Texture
{
    u32 texture;
    f32 brightness, rotation;
} Texture;

typedef struct TextureInstance
{
    Texture* inner;
    cstring name;
    f32 x, y;
    /**
     * @brief Since the game follows a strange kind of isometric projection
     * bullshit, we don't use a true up-and-down plane. Instead, we use a
     * webpage-like z-indexing system. This value can only go to 255, although
     * an index that high likely will never be seen in Renai.
     */
    u8 z;
} TextureInstance;

#define CreateTexture(path) CreateFullTexture(path, 1.0f, 0.0f)
#define CreateTextureWithBrightness(path, brightness)                          \
    CreateFullTexture(path, brightness, 0.0f)
#define CreateTextureWithRotation(path, rotation)                              \
    CreateFullTexture(path, 1.0f, rotation)
Texture* CreateFullTexture(cstring path, f32 brightness, f32 rotation);

#define RegisterTextureWithPosition(from, name, x, y)                          \
    RegisterFullTexture(from, name, x, y);
TextureInstance* RegisterFullTexture(Texture* from, cstring name, f32 x, f32 y,
                                     u8 z);

#endif
