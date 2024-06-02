/**
 * @file Texture.h
 * @author Zenais Argos
 * @brief Provides the functionality and data structures needed to load, render,
 * and register textures.
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_TEXTURE_
#define _RENAI_TEXTURE_

#include "Declarations.h"

typedef struct Texture
{
    u32 inner;
    u16 width, height;
    cstring name;
} Texture;

typedef struct TextureInstance
{
    Texture* inherits;
    f32 brightness, rotation;
    f32 x, y;
    u8 z;
} TextureInstance;

Texture* LoadTextureFromFile(cstring name, u16 width, u16 height);

#endif
