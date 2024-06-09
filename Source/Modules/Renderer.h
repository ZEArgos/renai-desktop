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

/**
 * @brief The renderer structure of the application. Includes all the data
 * needed to render objects, but NOT the objects to render. That is the job of
 * various other structures/functions.
 */
typedef struct Renderer
{

    LinkedList* shader_list;

    LinkedList* texture_list;
    /**
     * @brief The name of the renderer. This is really only used for
     * identification purposes.
     */
    u32 uid;
} Renderer;

/**
 * @brief The identifier of the main rendering interface.
 */
#define RENDERER_MAIN_UID 0x1283A

/**
 * @brief An empty initializer for the @ref Renderer struct. This simply just
 * makes sure everything is NULL.
 */
#define RENDERER_EMPTY_INIT                                                    \
    (struct Renderer) { NULL, NULL, 0 }

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
Renderer CreateRenderer(f32 swidth, f32 sheight, u32 uid);

/**
 * @brief Destroy a renderer and its contents.
 * @param renderer The renderer we are destroying.
 */
void DestroyRenderer(Renderer* renderer);

/**
 * @brief Check the validity of the given renderer. This basically just checks
 * to make sure nothing inside the renderer is uninitialized.
 * @param renderer The renderer to check.
 * @return The success flag of the function. SUCCESS means the renderer is
 * valid, FAILURE means it's invalid.
 */
u8 CheckRendererValidity(Renderer* renderer);

/**
 * @brief Render the contents of the game's window with the given renderer.
 * @param renderer The renderer in question.
 */
void RenderWindowContent(Renderer* renderer);

#endif // _RENAI_RENDERER_
