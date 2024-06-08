#include "Renderer.h" // The renderer mother header, declaring the various functions we define here.
#include "Logger.h" // Provides wrapper functions for logging to the standard output.
#include <cglm/cglm.h> // Provides the math functions and data structures needed to run complex graphical calculations.

/**
 * @brief Create the projection matrix for a shader/renderer. The projection
 * matrix used in Renai is an orthographic projection.
 * @param shader The shader into which to impose the matrix.
 * @param swidth The width of the screen.
 * @param sheight The height of the screen.
 * @return A success flag, either SUCCESS or FAILURE.
 */
u8 _CreateProjectionMatrix(ShaderNode* shader, f32 swidth, f32 sheight)
{
    // Assign the projection matrix to identity by default.
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    // Assemble the actual projection matrix, an orthographic projection with
    // the dimensions [0, 0, width, height].
    glm_ortho(0.0f, swidth, sheight, 0.0f, 0.0f, 1000.0f, projection);

    // In order to set the uniform, we need to set the basic shader to be
    // current. That's what we do here, if it fails, fail the application.
    if (!UseShader(shader->inner)) return FAILURE;
    // Set the matrix inside the shader and print any error that may have
    // happened.
    SetMat4(shader->inner, "projection", projection);
    if (!PrintGLError()) return FAILURE;

    // Return that nothing went wrong.
    PrintSuccess("Set a projection matrix inside shader '%s'.", shader->name);
    return SUCCESS;
}

/**
 * @brief Check if the head node of the given renderer and type is
 * invalid/uninitialized.
 * @param renderer The renderer to check.
 * @param type The type of list to check.
 * @return A success flag, SUCCESS representing a NULL list head, and the
 * inverse for FAILURE.
 */
u8 _CheckIfHeadInitialized(Renderer* renderer, NodeType type)
{
    // Get the list head of the given type for the given renderer, and check to
    // make sure it isn't null.
    if (GetListHead(renderer, type) != NULL)
    {
        // If it is, print a warning to the console and exit the function.
        PrintWarning("The %d list head of the given renderer was "
                     "already initialized.",
                     type);
        return FAILURE;
    }
    // Return our success.
    return SUCCESS;
}

Renderer CreateRenderer(f32 swidth, f32 sheight, u32 uid)
{
    // Initialize the renderer as empty right off the bat, and then assign a UID
    // to it.
    Renderer renderer = RENDERER_EMPTY_INIT;
    renderer.uid = uid;

    // Try to start the linked lists inside the renderer. If something goes
    // wrong, kill the function and return the equivalent of NULL. Otherwise,
    // print our success.
    if (!StartShaderList(&renderer, "basic") ||
        !StartTextureList(&renderer, "light_floorboard_1.jpg", swidth, sheight))
        return RENDERER_EMPTY_INIT;
    // Try to create the projection matrix of the renderer. If something goes
    // wrong, kill the function, just as for each of the linked lists.
    if (!_CreateProjectionMatrix(FindShaderNode(&renderer, "basic"), swidth,
                                 sheight))
        return RENDERER_EMPTY_INIT;

    // Print our success and exit the function.
    PrintSuccess("Initialized the renderer %d successfully.", uid);
    return renderer;
}

void DestroyRenderer(Renderer* renderer)
{
    // Clear the renderer's linked lists. These functions cannot reasonably
    // fail.
    ClearShaderList(renderer);
    ClearTextureList(renderer);
}

u8 CheckRendererValidity(Renderer* renderer)
{
    // Check if the renderer, its shader list head, or texture list head is
    // null. If none are, return a success code.
    if (renderer != NULL || renderer->shader_list_head != NULL ||
        renderer->texture_list_head != NULL)
        return SUCCESS;

    // Otherwise, print an error message and exit the function. Note that, due
    // to the fact that the renderer could be null, we don't give a certain UID.
    PrintError(
        "The given renderer (maybe %d?) is not valid. Please report this "
        "bug ASAP.",
        renderer->uid);
    return FAILURE;
}

void ClearLinkedList(Renderer* renderer, NodeType type)
{
    // Iterate over each element in the list, starting with the head and ending
    // with the end of the list.
    void* current_node = GetListHead(renderer, type);
    while (current_node != NULL)
    {
        // Store the next node in the list, as we will be destroying the current
        // node.
        void* next_node = GetNodeNext(type, current_node);
        // Destroy the current node.
        free(current_node);
        // Set the current node to the one we stored.
        current_node = next_node;
    }
}

void* FindNode(Renderer* renderer, NodeType type, const char* name)
{
    // Iterate over each element in the list, starting with the head node and
    // ending with the first node to not exist.
    void* current_node = GetListHead(renderer, type);
    while (current_node != NULL)
    {
        // Compare the name of the current node with the requesting name. If
        // this succeeds (0 = perfect match), return that node. Otherwise, get
        // the next node and repeat.
        if (strcmp(GetNodeName(type, current_node), name) == 0)
            return current_node;
        current_node = GetNodeNext(type, current_node);
    }
    // Print that we were unable to find a node with the requested name.
    PrintWarning("Couldn't find node '%s'.", name);
    return NULL;
}

void AppendNode(Renderer* renderer, NodeType type, void* node)
{
    // Iterate through the node list until we hit a node that doesn't exist yet.
    void* current_node = GetListHead(renderer, type);
    while (CheckNodeValidity(type, current_node))
        current_node = GetNodeNext(type, current_node);

    // Append the node onto the list.
    SetNodeNext(type, current_node, node);
}

void* GetListHead(Renderer* renderer, NodeType type)
{
    // Depending on the given type to find, return the list head of the
    // renderer.
    if (type == shader) return renderer->shader_list_head;
    else if (type == texture) return renderer->texture_list_head;
    return NULL;
}

u8 StartLinkedList(Renderer* renderer, NodeType type, const char* name,
                   f32 swidth, f32 sheight)
{
    // Check to make sure the list head of the given linked list type has not
    // already been initialized, and if it hasn't, continue with the function.
    if (type == shader && _CheckIfHeadInitialized(renderer, shader))
    {
        // Create a new shader node with the given name.
        renderer->shader_list_head = CreateShaderNode(name);
        // If that fails, print an error and return failure.
        if (renderer->shader_list_head == NULL)
        {
            PrintError("Failed to create the head shader node for renderer %d.",
                       renderer->uid);
            return FAILURE;
        }
        // Print our success and return from the function.
        PrintSuccess("Created head shader node for renderer %d.",
                     renderer->uid);
        return SUCCESS;
    }
    else if (type == texture && _CheckIfHeadInitialized(renderer, type))
    {
        // Create a new texture node from the given parameters.
        renderer->texture_list_head = CreateTextureNode(name, swidth, sheight);
        // If that fails, print error and return failure.
        if (renderer->texture_list_head == NULL)
        {
            PrintError(
                "Failed to create the head texture node for renderer %d.",
                renderer->uid);
            return FAILURE;
        }
        // Print our success and exit the function with no error code.
        PrintSuccess("Created head texture node for renderer %d.",
                     renderer->uid);
        return SUCCESS;
    }
    // If the head of the list has already been initialized, print a warning and
    // return failure.
    PrintWarning(
        "The head node(s) of the renderer %d have been initialized already.",
        renderer->uid);
    return FAILURE;
}

u8 CheckNodeValidity(NodeType type, void* node)
{
    // If the type of the node is shader and it (nor its linked part) isn't
    // NULL, return success.
    if (type == shader && node != NULL && ((ShaderNode*)node)->next != NULL)
        return SUCCESS;
    // If the type of node is texture and it + its individual parts aren't NULL,
    // return success.
    else if (type == texture && ((TextureNode*)node) != NULL &&
             ((TextureNode*)node)->next != NULL)
        return SUCCESS;
    // Return failure since something was NULL.
    return FAILURE;
}

void* GetNodeNext(NodeType type, void* node)
{
    // Cast the node to the needed type and return the next node in the list.
    if (type == shader) return ((ShaderNode*)node)->next;
    else if (type == texture) return ((TextureNode*)node)->next;
    else return NULL;
}

u8 SetNodeNext(NodeType type, void* node, void* subnode)
{
    if (node == NULL) return FAILURE;

    if (type == shader) ((ShaderNode*)node)->next = subnode;
    else if (type == texture) ((TextureNode*)node)->next = subnode;
    return SUCCESS;
}

const char* GetNodeName(NodeType type, void* node)
{
    if (type == shader) return ((ShaderNode*)node)->name;
    else if (type == texture) return ((TextureNode*)node)->name;
    else return NULL;
}

void RenderWindowContent(Renderer* renderer)
{
    if (!CheckRendererValidity(renderer)) exit(-1);

    // I'm not gonna bother documenting this function since its current state is
    // highly temporary.
    u32 basic_shader = FindShaderNode(renderer, "basic")->inner;
    if (!UseShader(basic_shader)) exit(-1);

    SetInteger(basic_shader, "in_texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->texture_list_head->inner.inner);

    glBindVertexArray(renderer->texture_list_head->inner.vao);

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){0.0f, 0.0f, 0.0f}); // pos transform

    SetMat4(basic_shader, "model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
