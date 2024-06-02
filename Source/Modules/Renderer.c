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
    f32 vertices[] = {
        // positions                        // colors
        0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, // top left
        0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f // top right
    };
    u32 indices[] = {0, 1, 2, 2, 3, 0};

    u32 VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    ShaderNode* shader = GetShader("basic");
    if (!shader->UseShader(shader))
        return FAILURE;

    glUniform1i(glGetUniformLocation(shader->shader, "in_texture"), 0);
    Texture* tex = LoadTextureFromFile(
        "./Assets/Tilesets/xanthos_outskirts_1.jpg", 32, 32);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->inner);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    free(tex);

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
