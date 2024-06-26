#include "Renderer.h"
#include <Logger.h>
#include <cglm/cglm.h>

/**
 * @brief Create the renderer's linked lists, and load their base resources.
 * @param renderer The renderer to write to.
 * @param width The width of the screen.
 * @param height The height of the screen.
 * @param caller The caller of the function.
 */
__INLINE __KILLFAIL _CreateLinkedLists(Renderer* renderer, f32 width,
                                       f32 height, const char* caller)
{
    renderer->shader_list =
        CreateLinkedList(shader, CreateShaderNode(shader, "basic"));
    renderer->texture_list = CreateLinkedList(
        texture,
        CreateTextureNode(texture, "texture_missing.jpg", width, height));

    // Make sure nothing went wrong.
    if (GetTextureListHead(renderer) == NULL ||
        GetShaderListHead(renderer) == NULL)
        PrintError("Failed to create the base resources of the renderer (are "
                   "files missing?).");
}

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
    _CreateLinkedLists(renderer, swidth, sheight, __func__);

    Node* basic_shader = GetRendererHead(renderer, Shader);
    // Create the projection matrix of the application, using a box with the
    // dimensions swidth x sheight x 1000.
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, swidth, sheight, 0.0f, 0.0f, 1000.0f, projection);

    // Slide the projection matrix into the shader.
    UseShader(*GetNodeContents(basic_shader, Shader));
    SetMat4(*GetNodeContents(basic_shader, Shader), "projection", projection);

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
        *GetNodeContents(GetRendererHead(renderer, Shader), Shader);
    UseShader(basic_shader);

    // Bind the "missing" texture to render as a placeholder.
    Texture* missing_texture =
        GetNodeContents(GetRendererHead(renderer, Texture), Texture);
    BindTexture(missing_texture);

    // Position transform the bound texture so it's within our viewport.
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});
    SetMat4(basic_shader, "model", model);

    // Draw the texture.
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
