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

/**
 * @brief The possible node types of the application. These are used in the
 * general Node___ methods.
 */
typedef enum NodeType
{
    /**
     * @brief A shader node, corresponding to the @ref ShaderNode type.
     */
    shader,
    /**
     * @brief A texture node, corresponding to the @ref TextureNode type.
     */
    texture
} NodeType;

/**
 * @brief Get the list head of the given type of object in the given renderer.
 * @param renderer The renderer to search.
 * @param type The type of linked list to look at.
 * @return A pointer to the list head, or NULL if something went wrong.
 */
void* GetListHead(Renderer* renderer, NodeType type);

/**
 * @brief Begin the given type of linked list for the given renderer. Using this
 * function as a user is cumbersome. Please use one of the helper definitions.
 * @param renderer The given renderer for which we are starting the list.
 * @param type The type of list to start.
 * @param name The name of the node we are inserting.
 * @param swidth The width of the primary monitor. @b Only needed for texture
 * list.
 * @param sheight The height of the primary monitor. @b Only needed for the
 * texture list.
 * @return An integer flag representing the success state of the function. This
 * can only be SUCCESS or FAILURE.
 */
u8 StartLinkedList(Renderer* renderer, NodeType type, const char* name,
                   f32 swidth, f32 sheight);

/**
 * @brief Start the shader linked list of the given renderer. This eliminates
 * the need to specify the extra arguments in the @ref StartLinkedList function
 * by hand.
 */
#define StartShaderList(renderer, name)                                        \
    StartLinkedList(renderer, shader, name, 0, 0)

/**
 * @brief Start the texture linked list of the given renderer.
 */
#define StartTextureList(renderer, name, swidth, sheight)                      \
    StartLinkedList(renderer, texture, name, swidth, sheight)

/**
 * @brief Clear a specific linked list within a @ref Renderer object.
 * @param renderer The renderer of which's linked list we are clearing.
 * @param type The type of linked list we want to clear.
 */
void ClearLinkedList(Renderer* renderer, NodeType type);

/**
 * @brief Clear specifically the shader list of the given renderer.
 */
#define ClearShaderList(renderer) ClearLinkedList(renderer, shader)

/**
 * @brief Clear specifically the texture list of the given renderer.
 */
#define ClearTextureList(renderer) ClearLinkedList(renderer, texture)

/**
 * @brief Find a node from a specific list in a specific renderer by name.
 * @param renderer The renderer from which we are trying to grab the node.
 * @param type The type of node we are trying to grab.
 * @param name The name of the node we are trying to grab.
 * @return A pointer to the node, or NULL if something went wrong.
 */
void* FindNode(Renderer* renderer, NodeType type, const char* name);

/**
 * @brief Find a shader node within the given renderer. This just automatically
 * converts the void* return of @ref FindNode to a @ref ShaderNode.
 */
#define FindShaderNode(renderer, name)                                         \
    ((ShaderNode*)FindNode(renderer, shader, name))

/**
 * @brief Find a texture node within the given renderer. Smiliar to @ref
 * FindShaderNode, this just automatically converts the void* return of @ref
 * FindNode to a @ref TextureNode.
 */
#define FindTextureNode(renderer, name)                                        \
    ((TextureNode*)FindNode(renderer, texture, name))

/**
 * @brief Append a node to the end of the given renderer's @param type linked
 * list.
 * @param renderer The "given renderer".
 * @param type The type of list to look through.
 * @param node The node to append.
 */
void AppendNode(Renderer* renderer, NodeType type, void* node);

/**
 * @brief Append specifically a shader node to the given renderer.
 */
#define AppendShaderNode(renderer, node) AppendNode(renderer, shader, node)

/**
 * @brief Append specifically a texture node to the given renderer.
 */
#define AppendTextureNode(renderer, node) AppendNode(renderer, texture, node)

/**
 * @brief Get the name of the given node in string form.
 * @param type The type of node the function's been given.
 * @param node The node to ask.
 * @return The string name of the node, or NULL if something went wrong.
 */
const char* GetNodeName(NodeType type, void* node);

/**
 * @brief Get the node linked to the given one.
 * @param type The type of node given.
 * @param node The node to check.
 * @return The node linked, or NULL if something went wrong.
 */
void* GetNodeNext(NodeType type, void* node);

/**
 * @brief Set the value of the node linked to the given one.
 * @param type The type of node we're setting.
 * @param node The node to link the @param subnode to.
 * @param subnode The node to be linked.
 * @return A success flag, either SUCCESS or FAILURE.
 */
u8 SetNodeNext(NodeType type, void* node, void* subnode);

/**
 * @brief Check a node's innards for inconsistencies with a fully initialized
 * node. @b Note that this does not check the node-type-specific components of
 * the given node, for that you have the node-specific helper functions.
 * @param type The type of node to check against.
 * @param node The node we're checking.
 * @return A success flag for the function, either SUCCESS or FAILURE.
 */
u8 CheckNodeValidity(NodeType type, void* node);

#endif // _RENAI_RENDERER_
