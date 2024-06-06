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
    u32 inner, vao;
    f32 width, height;
    const char* name;
} Texture;

/**
 * @brief A linked-list wrapper around a loaded texture. This is really not
 * particularly useful outside of the texture linked list used by the renderer.
 */
typedef struct TextureNode
{
    /**
     * @brief The next node in the list.
     */
    struct TextureNode* next;
    /**
     * @brief The string name of the texture. This is really only used for
     * lookup.
     */
    const char* name;
    /**
     * @brief The actual texture stored within in the node.
     */
    Texture inner;
} TextureNode;

typedef struct TextureInstance
{
    Texture* inherits;
    f32 brightness, rotation;
    f32 x, y;
    u8 z;
} TextureInstance;

TextureNode* CreateTextureNode(const char* texture_name, f32 swidth,
                               f32 sheight);

Texture LoadTextureFromFile(const char* name, f32 swidth, f32 sheight);

#define RegisterTexture(from, x, y, z)                                         \
    RegisterFullTexture(from, 1.0f, 0.0f, x, y, z)

TextureInstance RegisterFullTexture(Texture* from, f32 brightness, f32 rotation,
                                    f32 x, f32 y, u8 z);

#endif
