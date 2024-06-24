#include "Renderer.h"
#include "Logger.h"
#include <cglm/cglm.h>

/**
 * @brief A small macro just used to initialize the given renderer's linked
 * list. I've defined this solely to remove this ugly ass piece of code from the
 * @ref CreateRenderer function.
 */
#define CREATE_LINKED_LISTS(renderer)                                          \
    renderer->shader_list =                                                    \
        CreateLinkedList(shader, CreateShaderNode(shader, "basic"));           \
    renderer->texture_list = CreateLinkedList(                                 \
        texture,                                                               \
        CreateTextureNode(texture, "texture_missing.jpg", swidth, sheight))

__CREATE_STRUCT_KILLFAIL(Renderer)
CreateRenderer(f32 swidth, f32 sheight, const char* caller)
{
    Renderer* renderer = malloc(sizeof(Renderer));
    if (renderer == NULL)
        PrintError("Failed to allocate space for the application's renderer.");
    PrintSuccess("Allocated space for the application's renderer: %d bytes.",
                 sizeof(Renderer));

    // Create the renderer's various linked lists. This also stands to load the
    // base assets for the application.
    CREATE_LINKED_LISTS(renderer);

    Node* basic_shader = GetRendererHead(renderer, Shader);
    // Create the projection matrix of the application, using a box with the
    // dimensions swidth x sheight x 1000.
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, swidth, sheight, 0.0f, 0.0f, 1000.0f, projection);

    // Slide the projection matrix into the shader.
    UseShader(GetNodeContents(basic_shader, Shader));
    SetMat4(GetNodeContents(basic_shader, Shader), "projection", projection);

    PrintSuccess("Successfully set up the projection matrix on shader '%s'.",
                 basic_shader->name);

    return renderer;
}

void KillRenderer(Renderer* renderer)
{
    DestroyLinkedList(GetRendererList(renderer, Shader));
    DestroyLinkedList(GetRendererList(renderer, Texture));
    free(renderer);
}

void RenderWindowContent(Renderer* renderer)
{
    // Get the basic shader, the one we use to render plain textures, and slot
    // it as our current one.
    u32 basic_shader =
        GetNodeContents(GetRendererHead(renderer, Shader), Shader);
    UseShader(basic_shader);

    // Bind the "missing" texture to render as a placeholder.
    BindTexture(&GetNodeContents(GetRendererHead(renderer, Texture), Texture));

    // Position transform the bound texture so it's within our viewport.
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});
    SetMat4(basic_shader, "model", model);

    // Draw the texture.
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
