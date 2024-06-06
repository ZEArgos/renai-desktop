#include "Renderer.h" // The renderer mother header, declaring the various functions we define here.
#include "Logger.h" // Provides wrapper functions for logging to the standard output.
#include <glm/cglm.h> // Provides the math functions and data structures needed to run complex graphical calculations.

void ClearLinkedList(Renderer* renderer, u8 type)
{
    void* current_node;

    if (type == SHADER_NODE) current_node = renderer->shader_list_head;
    else if (type == TEXTURE_NODE) current_node = renderer->texture_list_head;
    else
    {
        PrintWarning("Attempted to clear a list that...doesn't exist. (%d)",
                     type);
        return;
    }

    while (current_node != NULL)
    {
        void* next_node;
        if (type == SHADER_NODE)
            next_node = ((ShaderNode*)(current_node))->next;
        else next_node = ((TextureNode*)(current_node))->next;

        free(current_node);
        current_node = next_node;
    }
}

void* FindNode(Renderer* renderer, u8 type, const char* name)
{
    void* current_node;

    if (type == SHADER_NODE) current_node = renderer->shader_list_head;
    else if (type == TEXTURE_NODE) current_node = renderer->texture_list_head;
    else
    {
        PrintWarning("Attempted to access a nonexistant node type. (%d)", type);
        return NULL;
    }

    while (current_node != NULL)
    {
        if (type == SHADER_NODE &&
            strcmp(((ShaderNode*)current_node)->name, name) == 0)
            return current_node;
        else if (strcmp(((TextureNode*)current_node)->name, name) == 0)
            return current_node;

        if (type == SHADER_NODE)
            current_node = ((ShaderNode*)current_node)->next;
        if (type == TEXTURE_NODE)
            current_node = ((TextureNode*)current_node)->next;
    }

    PrintWarning("Couldn't find node '%s'.", name);
    return NULL;
}

u8 AppendNode(Renderer* renderer, u8 type, void* node)
{
    void* current_node = GetListHead(renderer, type);

    if (type == SHADER_NODE)
    {
        while (((ShaderNode*)current_node) != NULL &&
               ((ShaderNode*)current_node)->next != NULL)
            current_node = ((ShaderNode*)current_node)->next;

        if ((ShaderNode*)current_node != NULL)
            ((ShaderNode*)current_node)->next = node;
        else
        {
            PrintWarning("Tried to start a linked list by appending a node. "
                         "This is illegal.");
            return FAILURE;
        }
    }
    else if (type == TEXTURE_NODE)
    {
        while (((TextureNode*)current_node)->next != NULL)
            current_node = ((TextureNode*)current_node)->next;
        ((TextureNode*)current_node)->next = node;
    }
    else
    {
        PrintWarning("Attempted to append a nonexistant node type. (%d)", type);
        return FAILURE;
    }

    return SUCCESS;
}

void* GetListHead(Renderer* renderer, u8 type)
{
    if (type == SHADER_NODE) return renderer->shader_list_head;
    else if (type == TEXTURE_NODE) return renderer->texture_list_head;
    else
    {
        PrintWarning("Attempted to find an nonexistent list head.");
        return NULL;
    }
}

void StartLinkedList(Renderer* renderer, u8 type, const char* name, f32 swidth,
                     f32 sheight)
{
    if (type == SHADER_NODE)
        renderer->shader_list_head = CreateShaderNode(name);
    else if (type == TEXTURE_NODE)
        renderer->texture_list_head = CreateTextureNode(name, swidth, sheight);
    else
    {
        PrintWarning("Attempted to access a start a linked list that doesn't "
                     "exist. (%d)",
                     type);
    }
}

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

    // Print our success.
    PrintSuccess("Set the projection matrix of the application.");

    return renderer;
}

void DestroyRenderer(Renderer* renderer)
{
    ClearLinkedList(renderer, SHADER_NODE);
    ClearLinkedList(renderer, TEXTURE_NODE);

    // Free the memory allocated by the renderer itself.
    free(renderer);
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
