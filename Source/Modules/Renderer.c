#include "Renderer.h"
#include "Logger.h"
#include "Shader.h"
#include "Window.h"

extern struct
{
    u8 initialized;
    Window* key_window;
    Renderer renderer;
} _application;

u32 GetShader(cstring shader_name)
{
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node != NULL)
    {
        if (strcmp(current_node->name, shader_name) == 0)
            return current_node->shader;
        current_node = current_node->next;
    }

    PrintWarning("Couldn't find shader '%s'.", shader_name);
    return 0;
}

null InsertShaderNode(ShaderNode* node)
{
    ShaderNode* current_node = _application.renderer.shader_list_head;
    while (current_node->next != NULL)
        current_node = current_node->next;

    current_node->next = node;
}

ShaderNode* CreateShaderNode(string shader_name)
{
    ShaderNode* created = malloc(sizeof(struct ShaderNode));
    created->next = NULL;
    created->shader = LoadShader(shader_name);
    created->name = shader_name;
    return created;
}

u8 InitializeRenderer(void)
{
    _application.renderer =
        (struct Renderer){.shader_list_head = CreateShaderNode("basic")};
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
