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

/**
 * @brief The max length a shader path can be. This is in place to prevent
 * buffer overflows.
 */
#define SHADER_PATH_MAX_LENGTH 128

/**
 * @brief Load a shader file from the 'Assets/Shaders/' folder. The "name" is
 * simply the name of the containing folder, everything else will be
 * automatically concatenated on.
 * @param name The shader's containing folder's name.
 * @return The OpenGL shader ID, or 0 if an error occurred.
 */
u32 LoadShader(const char* name);

/**
 * @brief Use the specified shader. Wrapper around @ref glUseProgram.
 * @param shader The shader to use.
 * @return An integer flag representing either success or failure.
 */
u8 UseShader(u32 shader);

/**
 * @brief Set a boolean variable inside the shader.
 * @param shader The shader we're changing.
 * @param name The name of the boolean.
 * @param value The new value of the boolean.
 */
void SetBoolean(u32 shader, const char* name, i8 value);

/**
 * @brief Set an integer variable inside the shader.
 * @param shader The shader we're changing.
 * @param name The name of the integer.
 * @param value The new value of the integer.
 */
void SetInteger(u32 shader, const char* name, i32 value);

/**
 * @brief Set a float variable inside the shader.
 * @param shader The shader we're changing.
 * @param name The name of the float.
 * @param value The new value of the float.
 */
void SetFloat(u32 shader, const char* name, f32 value);

#endif // _RENAI_SHADER_
