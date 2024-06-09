#include "Texture.h"
#include "Renderer.h"
#include "Window.h"
#include <cglm/cglm.h>
#include <stbi/stb_image.h>

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

Texture LoadTextureFromFile(const char* name, f32 swidth, f32 sheight)
{
    u32 texture_identifier;
    glGenTextures(1, &texture_identifier);
    glBindTexture(GL_TEXTURE_2D, texture_identifier);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    i32 image_width, image_height, image_channels;
    stbi_set_flip_vertically_on_load(1);

    //! strncat this
    char path[250] = "./Assets/Tilesets/";
    strcat(path, name);
    u8* data = stbi_load(path, &image_width, &image_height, &image_channels, 0);

    if (data != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else exit(-1);

    stbi_image_free(data);

    f32 vao_width = (swidth / image_width) * 4,
        vao_height = (sheight / image_height) * 4;
    float vertices[] = {
        // positions                                  // texture coords
        0.0f,       vao_height, 0.0f, 0.0f, 1.0f,      vao_width,  0.0f, 0.0f,
        1.0f,       0.0f,       0.0f, 0.0f, 0.0f,      0.0f,       0.0f, 0.0f,
        vao_height, 0.0f,       0.0f, 1.0f, vao_width, vao_height, 0.0f, 1.0f,
        1.0f,       vao_width,  0.0f, 0.0f, 1.0f,      0.0f};

    u32 VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return (struct Texture){texture_identifier, VAO, vao_width, vao_height,
                            name};
}

TextureInstance RegisterFullTexture(Texture* from, f32 brightness, f32 rotation,
                                    f32 x, f32 y, u8 z)
{
    return (struct TextureInstance){from, brightness, rotation, x, y, z};
}
