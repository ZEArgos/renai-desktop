#include "Renderer.h"
#include "Logger.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"

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
    Texture tex = LoadTextureFromFile(
        "./Assets/Tilesets/xanthos_outskirts_1.jpg", 0.25f, 0.25f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.inner);

    glBindVertexArray(tex.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &tex.vao);
    return SUCCESS;
}

u8 InitializeRenderer(void)
{
    _application.renderer = (struct Renderer){CreateShaderNode("basic"),
                                              _Renderer_RenderWindowContent};
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
