#include "Renderer.h"
#include "Logger.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include <glm/cglm.h>
#include <glm/common.h>
#include <glm/util.h>

extern struct
{
    u8 initialized;
    Window* key_window;
    Renderer renderer;
} _application;

ShaderNode* GetShader(cstring shader_name)
{
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node != NULL)
    {
        if (strcmp(current_node->name, shader_name) == 0)
            return current_node;
        current_node = current_node->next;
    }

    PrintWarning("Couldn't find shader '%s'.", shader_name);
    return NULL;
}

null InsertShaderNode(ShaderNode* node)
{
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node->next != NULL)
        current_node = current_node->next;

    current_node->next = node;
}

u8 _ShaderNode_UseShader(ShaderNode* self)
{
    if (!UseShader(self->shader))
        return FAILURE;
    return SUCCESS;
}

ShaderNode* CreateShaderNode(string shader_name)
{
    ShaderNode* created = malloc(sizeof(struct ShaderNode));
    created->next = NULL;
    created->shader = LoadShader(shader_name);
    created->name = shader_name;
    created->UseShader = _ShaderNode_UseShader;
    return created;
}

u8 _Renderer_RenderWindowContent(Renderer* self)
{
    ShaderNode* shader = GetShader("basic");
    if (!shader->UseShader(shader))
        return FAILURE;

    glUniform1i(glGetUniformLocation(shader->shader, "in_texture"), 0);
    Texture tex =
        LoadTextureFromFile("./Assets/Tilesets/xanthos_outskirts_1.jpg");
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
    glUniformMatrix4fv(glGetUniformLocation(shader->shader, "model"), 1,
                       GL_FALSE, &model[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteVertexArrays(1, &tex.vao);
    return SUCCESS;
}

u8 InitializeRenderer(f32 swidth, f32 sheight)
{
    _application.renderer = (struct Renderer){CreateShaderNode("basic"),
                                              _Renderer_RenderWindowContent};

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(60.0f), swidth / sheight, 0.1f, 100.0f, projection);
    glm_translate_z(projection, -3.0f);

    if (!_application.renderer.shader_list_head->UseShader(
            _application.renderer.shader_list_head))
        return FAILURE;
    glUniformMatrix4fv(
        glGetUniformLocation(_application.renderer.shader_list_head->shader,
                             "projection"),
        1, GL_FALSE, &projection[0][0]);

    PrintSuccess("Initialized the application renderer successfully.");
    return SUCCESS;
}

null DestroyRenderer(void)
{
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node != NULL)
    {
        ShaderNode* next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
}
