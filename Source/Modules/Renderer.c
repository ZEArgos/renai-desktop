#include "Renderer.h"
#include <Logger.h>
#include <cglm/cglm.h>
#include <stbi/stb_image.h>

/**
 * @brief Create the renderer's linked lists, and load their base
 * resources.
 * @param renderer The renderer to write to.
 * @param width The width of the screen.
 * @param height The height of the screen.
 */
__INLINE __KILLFAIL _CreateLinkedLists(Renderer* renderer,
                                       f32 window_width,
                                       f32 window_height)
{
    renderer->shader_list =
        CreateLinkedList(CreateShaderNode("basic"));

    // Make sure nothing went wrong.
    if (GetRendererHead(renderer, shader) == NULL)
        PrintError("Failed to create the base resources of the "
                   "renderer (are files missing?).");
}

__CREATE_STRUCT_KILLFAIL(Renderer)
CreateRenderer(f32 window_width, f32 window_height)
{
    Renderer* renderer =
        __MALLOC(Renderer, renderer,
                 ("Failed to allocate space for the application's "
                  "renderer. Code: %d.",
                  errno));
    PrintSuccess(
        "Allocated space for the application's renderer: %d bytes.",
        sizeof(Renderer));

    // Flip the textures loaded from STBI vertically, as otherwise
    // it'll load them upside down.
    stbi_set_flip_vertically_on_load(1);
    // Create the renderer's various linked lists. This also stands to
    // load the base assets for the application.
    _CreateLinkedLists(renderer, window_width, window_height);

    Node* basic_shader = GetRendererHead(renderer, shader);
    // Create the projection matrix of the application, using a box
    // with the dimensions swidth x sheight x 1000.
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, window_width, window_height, 0.0f, 0.0f, 1000.0f,
              projection);

    // Slide the projection matrix into the shader.
    UseShader(GetNodeContents(basic_shader, shader)->shader);
    SetMat4(GetNodeContents(basic_shader, shader)->shader,
            "projection", projection);
    PrintSuccess(
        "Successfully set up the projection matrix on shader '%s'.",
        basic_shader->name);

    renderer->scene_manager =
        CreateManager(window_width, window_height);

    return renderer;
}

void RenderWindowContent(Renderer* renderer)
{
    // Get the basic shader, the one we use to render plain textures,
    // and slot it as our current one.
    u32 basic_shader =
        GetNodeContents(GetRendererHead(renderer, shader), shader)
            ->shader;
    UseShader(basic_shader);

    RenderCurrentScene(
        renderer->scene_manager,
        GetNodeContents(GetRendererHead(renderer, shader), shader));
}
