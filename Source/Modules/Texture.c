#include "Texture.h"
#include <glm/cglm.h>
#include <stbi/stb_image.h>

Texture LoadTextureFromFile(cstring name, f32 swidth, f32 sheight)
{
    u32 texture_identifier;
    glGenTextures(1, &texture_identifier);
    glBindTexture(GL_TEXTURE_2D, texture_identifier);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    i32 image_width, image_height, image_channels;
    stbi_set_flip_vertically_on_load(1);
    u8* data = stbi_load(name, &image_width, &image_height, &image_channels, 0);

    if (data != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        exit(-1);

    stbi_image_free(data);

    f32 vao_width = image_width / swidth * 5,
        vao_height = image_height / sheight / 2 * 5;
    float vertices[] = {
        // positions                  // texture coords
        vao_width,  vao_height,  0.0f, 1.0f, 1.0f, // top right
        vao_width,  -vao_height, 0.0f, 1.0f, 0.0f, // bottom right
        -vao_width, -vao_height, 0.0f, 0.0f, 0.0f, // bottom left
        -vao_width, vao_height,  0.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3,                                   // first triangle
        1, 2, 3                                    // second triangle
    };

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return (struct Texture){texture_identifier, VAO, image_width, image_height,
                            name};
}

TextureInstance RegisterFullTexture(Texture* from, f32 brightness, f32 rotation,
                                    f32 x, f32 y, u8 z)
{
    return (struct TextureInstance){from, brightness, rotation, x, y, z};
}
