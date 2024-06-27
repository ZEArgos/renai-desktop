/**
 * @file Texture.h
 * @author Zenais Argos
 * @brief Provides the functionality and data structures needed to
 * load, render, and register textures.
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_TEXTURE_
#define _RENAI_TEXTURE_

#include <Declarations.h>
#include <Logger.h>

typedef enum TextureType
{
    tileset,
    sprite,
    render
} TextureType;

typedef struct Texture
{
    TextureType type;
    u16 width, height;
    u32 texture, vao;
    const char* name;
} Texture;

typedef struct TextureInstance
{
    Texture* inherits;
    f32 brightness, rotation;
    u8 scale;
    f32 x, y;
    u8 z;
} TextureInstance;

#define TEXTURE_EMPTY_INIT                                           \
    (struct Texture) { 0, 0, 0.0f, 0.0f, "" }

#define __TEXTURE_PATH_MAXLENGHT 128

Texture* LoadTextureFromFile(const char* name, TextureType type,
                             f32 swidth, f32 sheight,
                             const char* caller);

__INLINE void KillTexture(Texture* texture)
{
    PrintWarning("Freed the texture '%s'.", texture->name);
    free(texture);
}

#define RegisterTexture(from, x, y, z)                               \
    RegisterFullTexture(from, 1.0f, 0.0f, x, y, z)

TextureInstance RegisterFullTexture(Texture* from, f32 brightness,
                                    f32 rotation, f32 x, f32 y, u8 z);

__INLINE void BindTexture(Texture* texture)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBindVertexArray(texture->vao);
}

#endif
