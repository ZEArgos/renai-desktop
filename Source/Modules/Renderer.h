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

/**
 * @brief Get a shader node from the renderer's linked list by name.
 * @param shader_name The name of the shader, set by default as the name of the
 * shader's containing folder.
 * @return A pointer to the given shader node, or NULL if we couldn't find it.
 */
ShaderNode* GetShaderNode(Renderer* renderer, const char* shader_name);

TextureNode* GetTextureNode(Renderer* renderer, const char* texture_name);

/**
 * @brief Append a shader node to the renderer's linked list.
 * @param node A pointer to the memory containing the to-be-inserted node.
 */
void AppendShaderNode(Renderer* renderer, ShaderNode* node);

void AppendTextureNode(Renderer* renderer, TextureNode* node);

#endif // _RENAI_RENDERER_
