/**
 * @file stb_image.h
 * @author Zenais Argos (+ the contributors to STB Image)
 * @brief A heavily modified version of the STB Image library file,
 * with content I don't use cut. See
 * github.com/nothings/stb/blob/master/stb_image.h for both
 * contribution and licensing data. Note that the libstbi-###.a
 * file(s) include the full source code, this is simply contracted for
 * ease of use.
 * @date 2024-06-27
 *
 * @copyright Copyright (c) 2024
 */

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STBI_INCLUDE_STB_IMAGE_H

// Used for portable type capacity.
#include <stdint.h>

typedef uint8_t stbi_uc;
typedef uint16_t stbi_us;
typedef int32_t stbi_i;

enum
{
    STBI_default = 0,
    STBI_grey = 1,
    STBI_grey_alpha = 2,
    STBI_rgb = 3,
    STBI_rgb_alpha = 4
};

typedef struct stbi_io_callbacks
{
    stbi_i (*read)(void* user, char* data, stbi_i size);
    void (*skip)(void* user, stbi_i n);
    stbi_i (*eof)(void* user);
} stbi_io_callbacks;

extern stbi_uc* stbi_load(char const* filename, stbi_i* x, stbi_i* y,
                          stbi_i* channels_in_file,
                          stbi_i desired_channels);

extern stbi_uc* stbi_load_from_memory(stbi_uc const* buffer, int len,
                                      int* x, int* y,
                                      int* channels_in_file,
                                      int desired_channels);

extern const char* stbi_failure_reason(void);

extern void stbi_image_free(void* retval_from_stbi_load);

extern void
stbi_set_flip_vertically_on_load(stbi_i flag_true_if_should_flip);

#endif // STBI_INCLUDE_STB_IMAGE_H
