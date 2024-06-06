/**
 * @file Renderer.h
 * @author Zenais Argos
 * @brief Provides the functionality needed to render objects / images onto the
 * window.
 * @date 2024-05-27
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_RENDERER_
#define _RENAI_RENDERER_

// Provides the data structures and functions needed to manage OpenGL shaders
// within the application.
#include "Shader.h"
// Provides the various data structures and functions to handle textures.
#include "Texture.h"

/**
 * @brief The renderer structure of the application. Includes all the data
 * needed to render objects, but NOT the objects to render. That is the job of
 * various other structures/functions.
 */
typedef struct Renderer
{
    /**
     * @brief The head node of the shader linked list. This should NOT be
     * accessed or edited by the user, instead leave that to the various helper
     * functions.
     */
    ShaderNode* shader_list_head;
    /**
     * @brief The first node in the texture linked list. This should not be
     * edited or accessed by the user, leave that to the helper functions
     * provided.
     */
    TextureNode* texture_list_head;
} Renderer;

#define RENDERER_EMPTY_INIT                                                    \
    (struct Renderer) { NULL, NULL }

#define SHADER_NODE  0x011
#define TEXTURE_NODE 0x012
void ClearLinkedList(Renderer* renderer, u8 type);

#define FindShaderNode(renderer, name)                                         \
    ((ShaderNode*)FindNode(renderer, SHADER_NODE, name))

void* FindNode(Renderer* renderer, u8 type, const char* name);

u8 AppendNode(Renderer* renderer, u8 type, void* node);

void* GetListHead(Renderer* renderer, u8 type);

#define StartShaderList(renderer, name)                                        \
    StartLinkedList(renderer, SHADER_NODE, name, 0, 0);
#define StartTextureList(renderer, name, swidth, sheight)                      \
    StartLinkedList(renderer, TEXTURE_NODE, name, swidth, sheight);

void StartLinkedList(Renderer* renderer, u8 type, const char* name, f32 swidth,
                     f32 sheight);

/**
 * @brief Initialize the application's renderer. There is no need to pass
 * anything to this function, as it accesses the application structure itself.
 * This kills the application on error.
 */
Renderer* InitializeRenderer(f32 swidth, f32 sheight);

/**
 * @brief Destroy the application's renderer, returning it to nothing. This is
 * required for the application to close properly, as many of the list items are
 * dynamically allocated.
 */
void DestroyRenderer(Renderer* renderer);

/**
 * @brief Render the contents of the application's window. This should only be
 * called once every frame. This will kill the application on failure.
 */
void RenderWindowContent(Renderer* renderer, f32 swidth, f32 sheight);

#endif // _RENAI_RENDERER_
