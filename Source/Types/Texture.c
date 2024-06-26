#include "Texture.h"
#include <cglm/cglm.h>
#include <stbi/stb_image.h>

#define __SQUARE_VAO(width, height)                                            \
    {                                                                          \
        width, 0.0f,   0.0f, 1.0f, 1.0f, /* top right */                       \
        width, height, 0.0f, 1.0f, 0.0f, /* bottom right */                    \
        0.0f,  height, 0.0f, 0.0f, 0.0f, /* bottom left */                     \
        0.0f,  0.0f,   0.0f, 0.0f, 1.0f  /* top left */                        \
    };
#define __SQUARE_INDICES {0, 1, 3, 1, 2, 3};

Texture* LoadTextureFromFile(const char* name, f32 swidth, f32 sheight)
{
    Texture* texture = malloc(sizeof(struct Texture));
    texture->name = name;

    glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    i32 image_width, image_height, image_channels;
    stbi_set_flip_vertically_on_load(1);

    //! strncat this
    char path[250] = "./Assets/Tilesets/";
    strcat(path, name);
    u8* data = stbi_load(path, &image_width, &image_height, &image_channels, 0);

    //! there is an issue here with texture renderering; the textures are not
    //! the correct proportions/alignments
    if (data != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else exit(-1);

    stbi_image_free(data);

    texture->width = (swidth / image_width) * 4;
    texture->height = (sheight / image_height) * 4;
    f32 vertices[] = __SQUARE_VAO(texture->width, texture->height);
    u32 indices[] = __SQUARE_INDICES;

    u32 VBO, EBO;
    glGenVertexArrays(1, &texture->vao);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(texture->vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return texture;
}

TextureInstance RegisterFullTexture(Texture* from, f32 brightness, f32 rotation,
                                    f32 x, f32 y, u8 z)
{
    return (struct TextureInstance){from, brightness, rotation, x, y, z};
}
