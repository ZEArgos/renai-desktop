#include "Texture.h"
#include <stbi/stb_image.h>

Texture* LoadTextureFromFile(cstring name, u16 width, u16 height)
{
    u32 texture_identifier;
    glGenTextures(1, &texture_identifier);
    glBindTexture(GL_TEXTURE_2D, texture_identifier);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    i32 image_width, image_height, image_channels;
    stbi_set_flip_vertically_on_load(1);
    u8* data = stbi_load(name, &image_width, &image_height, &image_channels, 0);

    if (data != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        exit(-1);
    stbi_image_free(data);

    Texture* tex = malloc(sizeof(struct Texture));
    tex->inner = texture_identifier;
    tex->height = height;
    tex->width = width;
    tex->name = name;

    return tex;
}
