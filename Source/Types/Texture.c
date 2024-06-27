#include "Texture.h"
#include <cglm/cglm.h>
#include <stbi/stb_image.h>

#define __SQUARE_VAO(width, height)                                  \
    {                                                                \
        width, 0.0f,   0.0f, 1.0f, 1.0f, /* top right */             \
        width, height, 0.0f, 1.0f, 0.0f, /* bottom right */          \
        0.0f,  height, 0.0f, 0.0f, 0.0f, /* bottom left */           \
        0.0f,  0.0f,   0.0f, 0.0f, 1.0f  /* top left */              \
    };
#define __SQUARE_INDICES {0, 1, 3, 1, 2, 3};

#define __TYPE_STRING(type)                                          \
    (type == tileset  ? "Tilesets"                                   \
     : type == sprite ? "Sprites"                                    \
                      : "Renders")

#define __SET_TEXTURE_PARAMETERS                                     \
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,                \
                    GL_CLAMP_TO_BORDER);                             \
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,                \
                    GL_CLAMP_TO_BORDER);                             \
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,            \
                    GL_NEAREST_MIPMAP_NEAREST);                      \
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

u8* _LoadImageData(const char* path, i32* width, i32* height)
{
    i32 image_channels = 0;
    u8* data = stbi_load(path, width, height, &image_channels, 0);
    if (data == NULL)
    {
        PrintWarning("Failed to load the data of the image at '%s'. "
                     "Reason: %s.",
                     path, stbi_failure_reason());
        return NULL;
    }

    if (image_channels == 3) glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    else glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D, 0,
                 (image_channels == 3 ? GL_RGB : GL_RGBA), *width,
                 *height, 0, (image_channels == 3 ? GL_RGB : GL_RGBA),
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return data;
}

Texture* LoadTextureFromFile(const char* name, TextureType type,
                             f32 swidth, f32 sheight,
                             const char* caller)
{
    Texture* texture = malloc(sizeof(struct Texture));
    __CHECK_MALLOC(texture,
                   ("Failed to allocate the texture '%s'. Code: %d.",
                    name, errno));
    texture->name = name;

    glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    __SET_TEXTURE_PARAMETERS;

    char file_path[__TEXTURE_PATH_MAXLENGHT] = "./Assets/";
    snprintf(file_path + strlen(file_path), __TEXTURE_PATH_MAXLENGHT,
             "%s/%s", __TYPE_STRING(type), name);

    i32 image_width, image_height;
    _LoadImageData(file_path, &image_width, &image_height);
    texture->width = (swidth / image_width) * 4;
    texture->height = (sheight / image_height) * 4;

    f32 vertices[] = __SQUARE_VAO(texture->width, texture->height);
    u32 indices[] = __SQUARE_INDICES;

    u32 VBO, EBO;
    glGenVertexArrays(1, &texture->vao);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex
    // buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(texture->vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          NULL);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return texture;
}

TextureInstance RegisterFullTexture(Texture* from, f32 brightness,
                                    f32 rotation, f32 x, f32 y, u8 z)
{
    return (struct TextureInstance){from, brightness, rotation,
                                    x,    y,          z};
}
