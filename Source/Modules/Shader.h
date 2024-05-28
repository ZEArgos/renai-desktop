/**
 * @file Shader.h
 * @author Zenais Argos
 * @brief Provides the functionality needed for the application to load and
 * compile OpenGL shader files.
 * @date 2024-05-27
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_SHADER_
#define _RENAI_SHADER_

// Again, gimme them typedefs
#include "Declarations.h"

#define SHADER_PATH_MAX_LENGTH 128

u32 LoadShader(string name);

u8 UseShader(u32 shader);

null SetBoolean(u32 shader, cstring name, i8 value);

null SetInteger(u32 shader, cstring name, i32 value);

null SetFloat(u32 shader, cstring name, f32 value);

#endif // _RENAI_SHADER_
