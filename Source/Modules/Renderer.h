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

// Provides the various type definitions needed for this program to compile.
#include "Declarations.h"

/**
 * @brief A linked-list wrapper around an OpenGL shader. This is really not
 * particularly useful outside of the shader linked list used by the renderer.
 */
typedef struct ShaderNode
{
    /**
     * @brief The next node in the list.
     */
    struct ShaderNode* next;
    /**
     * @brief The string name of the shader. This is really only used for
     * lookup.
     */
    const char* name;
    /**
     * @brief The actual OpenGL shader ID of the node. This is the real meat of
     * the operation.
     */
    u32 inner;
} ShaderNode;

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
} Renderer;

/**
 * @brief Initialize the application's renderer. There is no need to pass
 * anything to this function, as it accesses the application structure itself.
 * This kills the application on error.
 */
void InitializeRenderer(void);

/**
 * @brief Destroy the application's renderer, returning it to nothing. This is
 * required for the application to close properly, as many of the list items are
 * dynamically allocated.
 */
void DestroyRenderer(void);

/**
 * @brief Render the contents of the application's window. This should only be
 * called once every frame. This will kill the application on failure.
 */
void RenderWindowContent(void);

/**
 * @brief Get a shader node from the renderer's linked list by name.
 * @param shader_name The name of the shader, set by default as the name of the
 * shader's containing folder.
 * @return A pointer to the given shader node, or NULL if we couldn't find it.
 */
ShaderNode* GetShaderNode(const char* shader_name);

/**
 * @brief Append a shader node to the renderer's linked list.
 * @param node A pointer to the memory containing the to-be-inserted node.
 */
void AppendShaderNode(ShaderNode* node);

/**
 * @brief Create a shader node from the given file name. This calls the @ref
 * LoadShader functions internally.
 * @param shader_name The name of the shader.
 * @return A pointer to the memory of the shader node. Note that this needs to
 * be freed, as it was allocated dynamically.
 */
ShaderNode* CreateShaderNode(const char* shader_name);

#endif // _RENAI_RENDERER_
