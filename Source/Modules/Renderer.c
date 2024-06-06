#include "Renderer.h"
#include "Logger.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include <glm/cglm.h>

/**
 * @brief The internal application structure. This struct is defined in
 * Application.c, so we just define it as an external variable here and call it
 * a day.
 */
extern struct
{
    u8 initialized;
    f32 screen_width, screen_height;
    Window window;
    Renderer renderer;
} _application;

void InitializeRenderer(void)
{
    // Setup the renderer by assembling the beginnings of each of its linked
    // lists.
    _application.renderer = (struct Renderer){CreateShaderNode("basic")};
    PrintSuccess("Initialized the application renderer successfully.");

    // Setup the projection matrix, making certain to initialize it to identity
    // before we do any fancy math on it.
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    // Create the orthographic projection matrix with the data we've assembled
    // so far, forming the last real base component of the rendering system.
    glm_ortho(0.0f, _application.screen_width, _application.screen_height, 0.0f,
              0.0f, 1000.0f, projection);

    // If we can't manage to access the basic shader, kill the program, as
    // something's gone very, very wrong.
    if (!UseShader(GetShaderNode("basic")->inner))
        exit(-1);
    // Set the projection matrix inside of the shader. This is done only once.
    SetMat4(GetShaderNode("basic")->inner, "projection", projection);
    // Poll for any potential OpenGL errors. If none occured, continue without
    // fail.
    if (!PrintGLError())
        exit(-1);

    // Print our success.
    PrintSuccess("Set the projection matrix of the application.");
}

void DestroyRenderer(void)
{
    // Loop through the current nodes until we have no more to loop through.
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node != NULL)
    {
        // If the shader exists, set the next in line to the stored next link of
        // the list.
        ShaderNode* next_node = current_node->next;
        // Free the shader's data.
        free(current_node);
        // Move along in the cycle.
        current_node = next_node;
    }
}

void RenderWindowContent(void)
{
    // I'm not gonna bother documenting this function since its current state is
    // highly temporary.

    u32 basic_shader = GetShaderNode("basic")->inner;
    if (!UseShader(basic_shader))
        exit(-1);

    SetInteger(basic_shader, "in_texture", 0);
    Texture tex = LoadTextureFromFile(
        "./Assets/Tilesets/light_floorboard_1.jpg", _application.screen_width,
        _application.screen_height);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.inner);

    glBindVertexArray(tex.vao);

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});
    SetMat4(basic_shader, "model", model);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &tex.vao);
}

ShaderNode* GetShaderNode(const char* shader_name)
{
    // Loop through each node, until the given node does not exist.
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node != NULL)
    {
        // If the name of the node and the name we are told to look for match,
        // return the node we're currently processing. If not, continue to the
        // next iteration of the loop.
        if (strcmp(current_node->name, shader_name) == 0)
            return current_node;
        current_node = current_node->next;
    }

    // Print that we failed to find the node and return empty-handed.
    PrintWarning("Couldn't find shader node '%s'.", shader_name);
    return NULL;
}

void AppendShaderNode(ShaderNode* node)
{
    // Loop through the shader nodes until we hit the end of the linked list.
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node->next != NULL)
        current_node = current_node->next;

    // Add the node to the end of the list.
    current_node->next = node;
}

ShaderNode* CreateShaderNode(const char* shader_name)
{
    // Allocate enough space for the node dynamically, and make sure the next
    // slot is not full of garbage data.
    ShaderNode* created = malloc(sizeof(struct ShaderNode));
    created->next = NULL;
    // Load the actual OpenGL shader into memory.
    created->inner = LoadShader(shader_name);
    // Set the name of the shader to whatever value we were given.
    created->name = shader_name;
    // Return the newly created shader node.
    return created;
}
