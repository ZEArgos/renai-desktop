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

#include <LinkedList.h>

#define RendererTextureList renderer->texture_list
#define RendererShaderList  renderer->shader_list
#define RendererTextureListHead                                                \
    renderer->texture_list->first_node->texture_contents
#define RendererShaderListHead                                                 \
    renderer->shader_list->first_node->shader_contents
#define RendererTextureListHeadNode renderer->texture_list->first_node
#define RendererShaderListHeadNode  renderer->shader_list->first_node

/**
 * @brief The renderer structure of the application. Includes all the data
 * needed to render objects, but NOT the objects to render. That is the job of
 * various other structures/functions.
 */
typedef struct Renderer
{

    LinkedList* shader_list;

    LinkedList* texture_list;
} Renderer;

/**
 * @brief Create a renderer object. This mainly initializes the linked lists of
 * the renderer.
 * @param swidth The width of the primary monitor, to be used in texture and
 * projection calculations.
 * @param sheight The height of the primary monitor, to be used in texture and
 * projection calculations.
 * @param uid The unique identifier of the renderer. This is used for
 * identification purposes.
 * @return The renderer we've just created, or a renderer filled with NULL if
 * something went wrong.
 */
__CREATE_STRUCT(Renderer) CreateRenderer(f32 width, f32 height);

/**
 * @brief Destroy a renderer and its contents.
 * @param renderer The renderer we are destroying.
 */
void KillRenderer(Renderer* renderer);

#define CheckRendererValidity(renderer)                                        \
    __CheckRendererValidity(renderer, __func__)
/**
 * @brief Check the validity of the given renderer. This basically just checks
 * to make sure nothing inside the renderer is uninitialized.
 * @param renderer The renderer to check.
 * @return The success flag of the function. SUCCESS means the renderer is
 * valid, FAILURE means it's invalid.
 */
u8 __CheckRendererValidity(Renderer* renderer, const char* caller);

/**
 * @brief Render the contents of the game's window with the given renderer.
 * @param renderer The renderer in question.
 */
void RenderWindowContent(Renderer* renderer);

#endif // _RENAI_RENDERER_
