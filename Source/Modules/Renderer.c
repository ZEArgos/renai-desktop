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
u8 _CreateProjectionMatrix(Node* shader, f32 swidth, f32 sheight)
{
    // Assign the projection matrix to identity by default.
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    // Assemble the actual projection matrix, an orthographic projection with
    // the dimensions [0, 0, width, height].
    glm_ortho(0.0f, swidth, sheight, 0.0f, 0.0f, 1000.0f, projection);

    // In order to set the uniform, we need to set the basic shader to be
    // current. That's what we do here, if it fails, fail the application.
    if (!UseShader(shader->shader_contents)) return FAILURE;
    // Set the matrix inside the shader and print any error that may have
    // happened.
    SetMat4(shader->shader_contents, "projection", projection);
    if (!PrintGLError()) return FAILURE;

    // Return that nothing went wrong.
    // PrintSuccess("Set a projection matrix inside shader '%s'.",
    // shader->name);
    return SUCCESS;
}

__CREATE_STRUCTURE(Renderer) CreateRenderer(f32 swidth, f32 sheight, u32 uid)
{
    // Initialize the renderer as empty right off the bat, and then assign a UID
    // to it.
    Renderer* renderer = malloc(sizeof(Renderer));
    renderer->uid = uid;

    // Try to start the linked lists inside the renderer. If something goes
    // wrong, kill the function and return the equivalent of NULL. Otherwise,
    // print our success.
    renderer->shader_list =
        CreateLinkedList(shader, CreateShaderNode(shader, "basic"));
    renderer->texture_list = CreateLinkedList(
        texture,
        CreateTextureNode(texture, "light_floorboard_1.jpg", swidth, sheight));
    // Try to create the projection matrix of the renderer. If something goes
    // wrong, kill the function, just as for each of the linked lists.
    if (!_CreateProjectionMatrix(RendererShaderListHeadNode, swidth, sheight))
        return NULL;

    // Print our success and exit the function.
    // PrintSuccess("Initialized the renderer %d successfully.", uid);
    return renderer;
}

void KillRenderer(Renderer* renderer)
{
    // Clear the renderer's linked lists. These functions cannot reasonably
    // fail.
    DestroyLinkedList(RendererShaderList);
    DestroyLinkedList(RendererTextureList);
    free(renderer);
}

u8 CheckRendererValidity(Renderer* renderer)
{
    // Check if the renderer, its shader list head, or texture list head is
    // null. If none are, return a success code.
    if (renderer != NULL || RendererShaderList != NULL ||
        RendererTextureList != NULL)
        return SUCCESS;

    // Otherwise, print an error message and exit the function. Note that, due
    // to the fact that the renderer could be null, we don't give a certain UID.
    PrintError(
        "The given renderer (maybe %d?) is not valid. Please report this "
        "bug ASAP.",
        renderer->uid);
    return FAILURE;
}

void RenderWindowContent(Renderer* renderer)
{
    if (!CheckRendererValidity(renderer)) exit(-1);

    // I'm not gonna bother documenting this function since its current state is
    // highly temporary.
    u32 basic_shader = GetShaderNode(RendererShaderList, "basic");
    if (!UseShader(basic_shader)) exit(-1);

    SetInteger(basic_shader, "in_texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, RendererTextureListHead.inner);

    glBindVertexArray(RendererTextureListHead.vao);

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){0.0f, 0.0f, 0.0f}); // pos transform

    SetMat4(basic_shader, "model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
