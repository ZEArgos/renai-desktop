/**
 * @file Renderer.h
 * @author Zenais Argos
 * @brief Provides the functionality needed to render objects / images
 * onto the window.
 * @date 2024-05-27
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_RENDERER_
#define _RENAI_RENDERER_

// This file defines the structure of a linked list, and provides some
// various helper functions to make working with them easier. It also
// includes Declarations.h, so we don't bother including that.
#include <LinkedList.h>

/**
 * @brief Basically just a large container for the various things
 * we'll need to render objects onto a screen efficiently (i.e
 * shaders, textures, etc.).
 */
typedef struct Renderer
{
    /**
     * @brief A linked list of shaders. This list includes each and
     * every shader created by the program.
     */
    LinkedList* shader_list;
    /**
     * @brief A linked list of textures. Note that these are literally
     * just loaded images, for rendering, instead register an @ref
     * TextureInstance, which is a specific occurance of a texture
     * with its own dye, rotation, and position.
     */
    LinkedList* texture_list;
} Renderer;

/**
 * @brief Create a new renderer. This basically just initializes the
 * base resources of the renderer ('basic' shader and 'missing'
 * texture, among others) and then does some fancy projection math.
 * @param window_width The width of the window we're going to be
 * rendering onto.
 * @param window_height The height of the window we're going to be
 * rendering onto.
 * @return A pointer to the renderer we just created.
 */
__CREATE_STRUCT_KILLFAIL(Renderer)
CreateRenderer(f32 window_width, f32 window_height);

/**
 * @brief Destroy a renderer object. This frees up all space allocated
 * to the object, resources included.
 * @param renderer The renderer we're going to kill.
 */
void KillRenderer(Renderer* renderer);

/**
 * @brief Get a list of the specified type from the given renderer.
 */
#define GetRendererList(renderer, list) Get##list##List(renderer)

/**
 * @brief Get the texture list of the given renderer.
 * @param renderer The renderer to grab from.
 * @return The requested linked list.
 */
__INLINE LinkedList* GetTextureList(Renderer* renderer)
{
    return renderer->texture_list;
}

/**
 * @brief Get the shader list of the given renderer.
 * @param renderer The renderer to grab from.
 * @return The requested linked list.
 */
__INLINE LinkedList* GetShaderList(Renderer* renderer)
{
    return renderer->shader_list;
}

/**
 * @brief Get the head node of the given renderer's list of the given
 * type.
 */
#define GetRendererHead(renderer, list) Get##list##ListHead(renderer)

/**
 * @brief Get the head node of the given renderer's texture list.
 * @param renderer The renderer to grab from.
 * @return The node requested.
 */
__INLINE Node* GetTextureListHead(Renderer* renderer)
{
    return renderer->texture_list->first_node;
}

/**
 * @brief Get the head node of the given renderer's shader list.
 * @param renderer The renderer to grab from.
 * @return The node requested.
 */
__INLINE Node* GetShaderListHead(Renderer* renderer)
{
    return renderer->shader_list->first_node;
}

/**
 * @brief Render the content of whatever window whose context is set
 * to current.
 * @param renderer The renderer to use for the process.
 */
void RenderWindowContent(Renderer* renderer);

#endif // _RENAI_RENDERER_
