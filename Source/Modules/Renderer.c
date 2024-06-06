#include "Renderer.h" // The renderer mother header, declaring the various functions we define here.
#include "Logger.h" // Provides wrapper functions for logging to the standard output.
#include <glm/cglm.h> // Provides the math functions and data structures needed to run complex graphical calculations.

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
    {
        PrintError("Failed to begin the linked lists of the renderer.");
        return RENDERER_EMPTY_INIT;
    }
    PrintSuccess("Initialized the renderer %d successfully.", uid);

    // Assign the projection matrix to identity by default.
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    // Assemble the actual projection matrix, an orthographic projection with
    // the dimensions [0, 0, width, height].
    glm_ortho(0.0f, swidth, sheight, 0.0f, 0.0f, 1000.0f, projection);
    // In order to set the uniform, we need to set the basic shader to be
    // current. That's what we do here, if it fails, fail the application.
    if (!UseShader(FindShaderNode(&renderer, "basic")->inner))
        return RENDERER_EMPTY_INIT;
    // Set the matrix inside the shader.
    SetMat4(FindShaderNode(&renderer, "basic")->inner, "projection",
            projection);
    // Print any errors that may have happened.
    if (!PrintGLError()) return RENDERER_EMPTY_INIT;

    // Print our success and exit the function.
    PrintSuccess("Set the projection matrix of the renderer %d.", uid);
    return renderer;
}

void DestroyRenderer(Renderer* renderer)
{
    ClearShaderList(renderer);
    ClearTextureList(renderer);
}

u8 CheckRendererValidity(Renderer* renderer)
{

    if (renderer != NULL || renderer->shader_list_head != NULL ||
        renderer->texture_list_head != NULL)
        return SUCCESS;

    PrintError("The given renderer (%d?) is not valid. Please report this "
               "bug ASAP.",
               renderer->uid);
    return FAILURE;
}

void ClearLinkedList(Renderer* renderer, NodeType type)
{
    void* current_node = GetListHead(renderer, type);

    while (current_node != NULL)
    {
        void* next_node = GetNodeNext(type, current_node);

        free(current_node);
        current_node = next_node;
    }
}

void* FindNode(Renderer* renderer, NodeType type, const char* name)
{
    void* current_node = GetListHead(renderer, type);

    while (current_node != NULL)
    {
        if (strcmp(GetNodeName(type, current_node), name) == 0)
            return current_node;
        current_node = GetNodeNext(type, current_node);
    }

    PrintWarning("Couldn't find node '%s'.", name);
    return NULL;
}

u8 AppendNode(Renderer* renderer, NodeType type, void* node)
{
    void* current_node = GetListHead(renderer, type);

    while (CheckNodeValidity(type, current_node))
        current_node = GetNodeNext(type, current_node);

    if (node != NULL) SetNodeNext(type, current_node, node);
    else
    {
        PrintWarning("Tried to start a linked list by appending a node. "
                     "This is illegal.");
        return FAILURE;
    }

    return SUCCESS;
}

//! SUBFUNCTION SOME OF THIS SHIT OUT MAN
//!!!!!!!!

void* GetListHead(Renderer* renderer, NodeType type)
{
    if (type == shader) return renderer->shader_list_head;
    else if (type == texture) return renderer->texture_list_head;
    return NULL;
}

u8 _CheckIfHeadInitialized(void* head, NodeType type)
{
    if (head != NULL)
    {
        PrintWarning("The %s list head of the given renderer was "
                     "already initialized.",
                     (type == shader ? "shader" : "texture"));
        return FAILURE;
    }
    return SUCCESS;
}
u8 StartLinkedList(Renderer* renderer, NodeType type, const char* name,
                   f32 swidth, f32 sheight)
{
    if (type == shader &&
        _CheckIfHeadInitialized(renderer->shader_list_head, shader))
    {
        renderer->shader_list_head = CreateShaderNode(name);
        if (renderer->shader_list_head == NULL) return FAILURE;
        return SUCCESS;
    }
    else if (type == texture &&
             _CheckIfHeadInitialized(renderer->texture_list_head, type))
    {
        renderer->texture_list_head = CreateTextureNode(name, swidth, sheight);
        if (renderer->texture_list_head == NULL) return FAILURE;
        return SUCCESS;
    }

    return FAILURE;
}

u8 CheckNodeValidity(NodeType type, void* node)
{
    if (type == shader && ((ShaderNode*)node) != NULL &&
        ((ShaderNode*)node)->next != NULL)
        return SUCCESS;
    else if (type == texture && ((TextureNode*)node) != NULL &&
             ((TextureNode*)node)->next != NULL)
        return SUCCESS;
    return FAILURE;
}

void* GetNodeNext(NodeType type, void* node)
{
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
