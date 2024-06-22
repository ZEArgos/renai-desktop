#include "Renderer.h" // The renderer mother header, declaring the various functions we define here.
#include "Logger.h" // Provides wrapper functions for logging to the standard output.
#include <cglm/cglm.h> // Provides the math functions and data structures needed to run complex graphical calculations.

//! working here

__BOOLEAN _CreateProjectionMatrix(Node* shader, f32 swidth, f32 sheight)
{
    mat4 projection = GLM_MAT4_IDENTITY_INIT;

    glm_ortho(0.0f, swidth, sheight, 0.0f, 0.0f, 1000.0f, projection);

    UseShader(shader->shader_contents);

    SetMat4(shader->shader_contents, "projection", projection);
    PollOpenGLErrors(__func__);

    PrintSuccess("Successfully set up the projection matrix on shader '%s'.",
                 shader->name);

    return true;
}

__CREATE_STRUCT_KILLFAIL(Renderer)
CreateRenderer(f32 swidth, f32 sheight, const char* caller)
{
    Renderer* renderer = malloc(sizeof(Renderer));
    if (renderer == NULL)
        PrintError("Failed to allocate space for the application's renderer.");
    PrintSuccess("Allocated space for the application's renderer: %d bytes.",
                 sizeof(Renderer));

    renderer->shader_list =
        CreateLinkedList(shader, CreateShaderNode(shader, "basic"));
    renderer->texture_list = CreateLinkedList(
        texture,
        CreateTextureNode(texture, "texture_missing.jpg", swidth, sheight));
    if (!CheckRendererValidity(renderer, __func__))
        PrintError("Unable to create the linked lists for the application's "
                   "renderer.");

    if (!_CreateProjectionMatrix(RendererShaderListHeadNode, swidth, sheight))
        PrintError("Failed to create the projection matrix of the renderer.");

    return renderer;
}

void KillRenderer(Renderer* renderer)
{
    DestroyLinkedList(GetRendererList(renderer, Shader));
    DestroyLinkedList(GetRendererList(renderer, Texture));
    free(renderer);
}

__BOOLEAN CheckRendererValidity(Renderer* renderer, const char* caller)
{
    if (renderer != NULL || GetRendererList(renderer, Shader) != NULL ||
        GetRendererList(renderer, Texture) != NULL)
        return true;
    return false;
}

void RenderWindowContent(Renderer* renderer)
{
    if (!CheckRendererValidity(renderer, __func__)) exit(-1);

    u32 basic_shader =
        GetShaderNode(GetRendererList(renderer, Shader), "basic");
    UseShader(basic_shader);

    SetInteger(basic_shader, "in_texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, RendererTextureListHead.inner);

    glBindVertexArray(RendererTextureListHead.vao);

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){0.0f, 0.0f, 0.0f}); // pos transform

    SetMat4(basic_shader, "model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
