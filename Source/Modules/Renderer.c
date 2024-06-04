#include "Renderer.h"
#include "Logger.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include <glm/cglm.h>

/**
 * @brief The internal application structure. This is defined in Application.h,
 * so we just define it as external here and call it a day.
 */
extern struct
{
    u8 initialized;
    f32 screen_width, screen_height;
    f32 aspect_ratio;
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

    int width, height;
    glfwGetWindowSize(_application.window.inner_window, &width, &height);

    glm_ortho(0.0f, width, height, 0.0f, 0.0f, 1000.0f, projection);
    // glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);
    //   glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(200.0f));
    //   glm::mat4 VPmatrix = perspective * model;

    // Setup the perspective matrix with 60 FOV, using the primary monitor's
    // width and height.
    // glm_perspective(glm_rad(60.0f), 1, 0.1f, 100.0f, projection);
    // glm_translate_z(projection, -3.0f);

    if (!UseShader(_application.renderer.shader_list_head->inner))
        exit(-1);
    glUniformMatrix4fv(
        glGetUniformLocation(_application.renderer.shader_list_head->inner,
                             "projection"),
        1, GL_FALSE, &projection[0][0]);

    // Print our success.
    PrintSuccess("Initialized the projection matrix of the application.");
}

void DestroyRenderer(void)
{
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node != NULL)
    {
        ShaderNode* next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
}

void RenderWindowContent(void)
{
    ShaderNode* shader = GetShaderNode("basic");
    if (!UseShader(shader->inner))
        exit(-1);

    glUniform1i(glGetUniformLocation(shader->inner, "in_texture"), 0);
    Texture tex = LoadTextureFromFile(
        "./Assets/Tilesets/light_floorboard_1.jpg", _application.screen_width,
        _application.screen_height);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.inner);

    // render boxes
    glBindVertexArray(tex.vao);

    // world space positions of our cubes
    vec3 pos = {0.0f, 0.0f, 0.0f}, scale = {650.0f, 650.0f, 0.0f};
    // calculate the model matrix for each object and pass it to shader
    // before drawing
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, pos);
    glm_scale(model, scale);

    // float angle = 50.0f;
    // glm_rotate(model, glm_rad(angle), (vec3){1.0f, 1.0f, 1.0f});
    glUniformMatrix4fv(glGetUniformLocation(shader->inner, "model"), 1,
                       GL_FALSE, &model[0][0]);

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
