#include "Texture.h"
#include <cglm/cglm.h>
#include <stbi/stb_image.h>

#define __SQUARE_VAO(width, height)                                  \
    {                                                                \
        width, 0.0f, 0.0f, 1.0f, 1.0f,       /* top right */         \
            width, height, 0.0f, 1.0f, 0.0f, /* bottom right */      \
            0.0f, height, 0.0f, 0.0f, 0.0f,  /* bottom left */       \
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f     /* top left */          \
    }
#define __SQUARE_INDICES                                             \
    {                                                                \
        0, 1, 3, 1, 2, 3                                             \
    }

#define __TYPE_STRING(type)                                          \
    (type == tileset  ? "Tilesets"                                   \
     : type == sprite ? "Sprites"                                    \
                      : "Renders")

__INLINE void _InitializeOpenGLTexture(u32* texture, i32 wrap,
                                       i32 min, i32 max)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max);
}

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

__INLINE void _InsertTextureData(Texture* texture, const char* name,
                                 i32 width_ratio, i32 height_ratio)
{
    texture->name = (char*)name;
    texture->width = width_ratio * 4;
    texture->height = height_ratio * 4;
}

u32 _GenerateVertexBuffer(u16 texture_width, u16 texture_height)
{
    f32 vertices[] = __SQUARE_VAO(texture_width, texture_height);
    u32 indices[] = __SQUARE_INDICES;

    u32 VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

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

    return VAO;
}

#define __TEXTURE_PATH_MAXLENGTH 64

Texture* CreateTexture(const char* name, TextureType type,
                       f32 window_width, f32 window_height)
{
    Texture* texture =
        __MALLOC(Texture, texture,
                 ("Failed to allocate the texture '%s'. Code: %d.",
                  name, errno));

    char file_path[__TEXTURE_PATH_MAXLENGTH];
    snprintf(file_path, __TEXTURE_PATH_MAXLENGTH, "./Assets/%s/%s",
             __TYPE_STRING(type), name);

    _InitializeOpenGLTexture(&texture->texture, GL_CLAMP_TO_BORDER,
                             GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
    i32 image_width, image_height;
    _LoadImageData(file_path, &image_width, &image_height);
    _InsertTextureData(texture, name, window_width / image_width,
                       window_height / image_height);
    texture->vao =
        _GenerateVertexBuffer(texture->width, texture->height);

    PrintSuccess("Loaded texture from file '%s'.", file_path);
    return texture;
}

__CREATE_STRUCT(Texture)
CreateTextureFromMemory(const char* name, u8* image, u64 image_size,
                        TextureType type, f32 window_width,
                        f32 window_height)
{
    Texture* texture =
        __MALLOC(Texture, texture,
                 ("Failed to allocate the texture '%s'. Code: %d.",
                  name, errno));

    _InitializeOpenGLTexture(&texture->texture, GL_CLAMP_TO_BORDER,
                             GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);

    i32 image_width, image_height, image_channels;
    u8* image_content =
        stbi_load_from_memory(image, image_size, &image_width,
                              &image_height, &image_channels, 0);

    if (image_channels == 3) glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    else glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D, 0,
                 (image_channels == 3 ? GL_RGB : GL_RGBA),
                 image_width, image_height, 0,
                 (image_channels == 3 ? GL_RGB : GL_RGBA),
                 GL_UNSIGNED_BYTE, image_content);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image_content);

    _InsertTextureData(texture, name, window_width / image_width,
                       window_height / image_height);

    texture->vao =
        _GenerateVertexBuffer(texture->width, texture->height);

    PrintSuccess("Loaded texture '%s' from memory.", name);
    return texture;
}

__CREATE_STRUCT(TextureInstance)
RegisterTexture(Texture* from, f32 x, f32 y, u8 z, u8 scale,
                f32 brightness, f32 rotation)
{
    TextureInstance* registered_texture =
        __MALLOC(TextureInstance, registered_texture,
                 ("Failed to allocate enough space for a texture "
                  "instance inheriting from '%s'.",
                  from->name));

    registered_texture->inherits = from;
    registered_texture->x = x;
    registered_texture->y = y;
    registered_texture->z = z;
    registered_texture->scale = scale;
    registered_texture->brightness = brightness;
    registered_texture->rotation = rotation;

    return registered_texture;
}
