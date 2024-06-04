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
    Window window;
    Renderer renderer;
} _application;

ShaderNode* GetShaderNode(const char* shader_name)
{
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node != NULL)
    {
        if (strcmp(current_node->name, shader_name) == 0)
            return current_node;
        current_node = current_node->next;
    }

    PrintWarning("Couldn't find shader node '%s'.", shader_name);
    return NULL;
}

void InsertShaderNode(ShaderNode* node)
{
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node->next != NULL)
        current_node = current_node->next;

    current_node->next = node;
}

ShaderNode* CreateShaderNode(char* shader_name)
{
    ShaderNode* created = malloc(sizeof(struct ShaderNode));
    created->next = NULL;
    created->inner = LoadShader(shader_name);
    created->name = shader_name;
    return created;
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

    // world space positions of our cubes
    vec3 pos = {0.0f, 0.0f, 0.0f};

    // render boxes
    glBindVertexArray(tex.vao);

    // calculate the model matrix for each object and pass it to shader
    // before drawing
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, pos);

    // float angle = 50.0f;
    // glm_rotate(model, glm_rad(angle), (vec3){1.0f, 1.0f, 1.0f});
    glUniformMatrix4fv(glGetUniformLocation(shader->inner, "model"), 1,
                       GL_FALSE, &model[0][0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &tex.vao);
}

void InitializeRenderer(void)
{
    _application.renderer = (struct Renderer){
        CreateShaderNode("basic"),
    };

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(60.0f),
                    _application.screen_width / _application.screen_height,
                    0.1f, 100.0f, projection);
    glm_translate_z(projection, -3.0f);

    if (!UseShader(_application.renderer.shader_list_head->inner))
        exit(-1);
    glUniformMatrix4fv(
        glGetUniformLocation(_application.renderer.shader_list_head->inner,
                             "projection"),
        1, GL_FALSE, &projection[0][0]);

    PrintSuccess("Initialized the application renderer successfully.");
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
