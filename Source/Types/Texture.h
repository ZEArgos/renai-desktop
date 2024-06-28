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

/**
 * @brief Load a complete texture object from the given file,
 * OpenGL-ready bitmap and all.
 * @param name The file name of the image. Note that the full image
 * path cannot be more than 64 characters, or it will be truncated.
 * This means the image name can be, at most, 46 or 47 characters long
 * (depending on the type).
 * @param type The type of image it is.
 * @param window_width The width of the key window.
 * @param window_height The height of the key window.
 * @return A pointer to the created texture.
 */
__CREATE_STRUCT(Texture)
CreateTexture(const char* name, TextureType type, f32 window_width,
              f32 window_height);

/**
 * @brief Free all resources to do with the given texture.
 * @param texture The texture to kill.
 */
__INLINE void KillTexture(Texture* texture)
{
    const char* freed_name = texture->name;
    __FREE(texture,
           ("The texture freer was given an invalid texture."));
    PrintWarning("Freed the texture '%s'.", freed_name);
}

__INLINE void BindTexture(Texture* texture)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBindVertexArray(texture->vao);
}

#endif
