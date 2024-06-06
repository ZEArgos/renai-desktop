#include "Renderer.h" // The renderer mother header, declaring the various functions we define here.
#include "Logger.h" // Provides wrapper functions for logging to the standard output.
#include <glm/cglm.h> // Provides the math functions and data structures needed to run complex graphical calculations.

Renderer* InitializeRenderer(f32 swidth, f32 sheight)
{
    // Setup the renderer by assembling the beginnings of each of its linked
    // lists.
    Renderer* renderer = malloc(sizeof(struct Renderer));
    *renderer = RENDERER_EMPTY_INIT;

    StartShaderList(renderer, "basic");
    StartTextureList(renderer, "./Assets/Tilesets/light_floorboard_1.jpg",
                     swidth, sheight);

    PrintSuccess("Initialized the application renderer successfully.");

    // Setup the projection matrix, making certain to initialize it to identity
    // before we do any fancy math on it.
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    // Create the orthographic projection matrix with the data we've assembled
    // so far, forming the last real base component of the rendering system.
    glm_ortho(0.0f, swidth, sheight, 0.0f, 0.0f, 1000.0f, projection);

    // If we can't manage to access the basic shader, kill the program, as
    // something's gone very, very wrong.
    if (!UseShader(FindShaderNode(renderer, "basic")->inner)) exit(-1);
    // Set the projection matrix inside of the shader. This is done only once.
    SetMat4(FindShaderNode(renderer, "basic")->inner, "projection", projection);
    // Poll for any potential OpenGL errors. If none occured, continue without
    // fail.
    if (!PrintGLError()) exit(-1);

    // Print our success and return the renderer.
    PrintSuccess("Set the projection matrix of the application.");
    return renderer;
}

void DestroyRenderer(Renderer* renderer)
{
    // Clear the renderer's linked lists.
    ClearLinkedList(renderer, shader);
    ClearLinkedList(renderer, texture);

    // Free the memory allocated by the renderer itself.
    free(renderer);
}

void ClearLinkedList(Renderer* renderer, NodeType type)
{
    void* current_node = GetListHead(renderer, type);
    if (current_node == NULL) return;

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
    if (current_node == NULL) return NULL;

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
    if (current_node == NULL) return FAILURE;

    while (CheckFullNodeValidity(type, current_node))
        current_node = GetNodeNext(type, current_node);

    if (CheckShallowNodeValidity(type, node))
        SetNodeNext(type, current_node, node);
    else
    {
        PrintWarning("Tried to start a linked list by appending a node. "
                     "This is illegal.");
        return FAILURE;
    }

    return SUCCESS;
}

void* GetListHead(Renderer* renderer, NodeType type)
{
    if (type == shader) return renderer->shader_list_head;
    else if (type == texture) return renderer->texture_list_head;
    else
    {
        PrintWarning("Attempted to find an nonexistent list head.");
        return NULL;
    }
}

void StartLinkedList(Renderer* renderer, NodeType type, const char* name,
                     f32 swidth, f32 sheight)
{
    if (type == shader) renderer->shader_list_head = CreateShaderNode(name);
    else if (type == texture)
        renderer->texture_list_head = CreateTextureNode(name, swidth, sheight);
}

u8 CheckFullNodeValidity(NodeType type, void* node)
{
    if (type == shader && ((ShaderNode*)node) != NULL &&
        ((ShaderNode*)node)->next != NULL)
        return SUCCESS;
    else if (type == texture && ((TextureNode*)node) != NULL &&
             ((TextureNode*)node)->next != NULL)
        return SUCCESS;
    return FAILURE;
}
u8 CheckShallowNodeValidity(u8 type, void* node)
{
    if (type == shader && ((ShaderNode*)node) != NULL) return SUCCESS;
    else if (type == texture && ((TextureNode*)node) != NULL) return SUCCESS;
    return FAILURE;
}

void* GetNodeNext(NodeType type, void* node)
{
    if (type == shader) return ((ShaderNode*)node)->next;
    else if (type == texture) return ((TextureNode*)node)->next;
    else return NULL;
}
void SetNodeNext(NodeType type, void* node, void* subnode)
{
    if (type == shader) ((ShaderNode*)node)->next = subnode;
    else if (type == texture) ((TextureNode*)node)->next = subnode;
}
const char* GetNodeName(NodeType type, void* node)
{
    if (type == shader) return ((ShaderNode*)node)->name;
    else if (type == texture) return ((TextureNode*)node)->name;
    else return NULL;
}

void RenderWindowContent(Renderer* renderer, f32 swidth, f32 sheight)
{
    // I'm not gonna bother documenting this function since its current state is
    // highly temporary.
    u32 basic_shader = FindShaderNode(renderer, "basic")->inner;
    if (!UseShader(basic_shader)) exit(-1);

    SetInteger(basic_shader, "in_texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->texture_list_head->inner.inner);

    glBindVertexArray(renderer->texture_list_head->inner.vao);

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});
    SetMat4(basic_shader, "model", model);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
