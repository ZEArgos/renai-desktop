#include "Shader.h"
#include "Logger.h"

u32 LoadShader(string name)
{
    char vertex_path[SHADER_PATH_MAX_LENGTH],
        fragment_path[SHADER_PATH_MAX_LENGTH];

    snprintf(vertex_path, SHADER_PATH_MAX_LENGTH, "Assets/Shaders/%s/vertex.vs",
             name);
    snprintf(fragment_path, SHADER_PATH_MAX_LENGTH,
             "Assets/Shaders/%s/fragment.fs", name);

    FILE *vertex_file = fopen(vertex_path, "rb"),
         *fragment_file = fopen(fragment_path, "rb");

    if (vertex_file && fragment_file)
    {
        fseek(vertex_file, 0, SEEK_END);
        fseek(fragment_file, 0, SEEK_END);

        i64 vertex_length = ftell(vertex_file);
        i64 fragment_length = ftell(fragment_file);

        fseek(vertex_file, 0, SEEK_SET);
        fseek(fragment_file, 0, SEEK_SET);
        char* vertex_buffer = malloc(vertex_length + 1);
        char* fragment_buffer = malloc(fragment_length + 1);

        if (vertex_buffer && fragment_buffer)
        {
            fread(vertex_buffer, 1, vertex_length, vertex_file);
            fread(fragment_buffer, 1, fragment_length, fragment_file);
            vertex_buffer[vertex_length] = '\0';
            fragment_buffer[fragment_length] = '\0';

            fclose(vertex_file);
            fclose(fragment_file);

            cstring vertex_raw = vertex_buffer, fragment_raw = fragment_buffer;
            u32 vertex = glCreateShader(GL_VERTEX_SHADER),
                fragment = glCreateShader(GL_FRAGMENT_SHADER);

            glShaderSource(vertex, 1, &vertex_raw, NULL);
            glCompileShader(vertex);
            int success;
            char infoLog[1024];

            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
                printf("ERROR::vert. log: %s", infoLog);
            }

            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fragment_raw, NULL);
            glCompileShader(fragment);
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
                printf("ERROR::frag. log: %s", infoLog);
            }
            // shader Program
            u32 final = glCreateProgram();
            glAttachShader(final, vertex);
            glAttachShader(final, fragment);
            glLinkProgram(final);
            glGetProgramiv(final, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(final, 1024, NULL, infoLog);
                printf("ERROR::PROGRAM_COMPILATION_ERROR. log: %s", infoLog);
            }
            // delete the shaders as they're linked into our program now and no
            // longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            free(vertex_buffer);
            free(fragment_buffer);

            return final;
        }
        else
        {
            PrintError("Failed to allocate space for the vertex/fragment "
                       "shader buffer. Code: %d.",
                       errno);
            return FAILURE;
        }

        return SUCCESS;
    }
    PrintError("Failed to open vertex and/or fragment shader file for shader "
               "'%s'. Code: %d",
               name, errno);
    return FAILURE;
}

u8 UseShader(u32 shader)
{
    glUseProgram(shader);

    i32 err = glGetError();
    if (err != 0)
    {
        PrintError("Failed to use shader program. Code: %d.", err);
        return FAILURE;
    }
    return SUCCESS;
}

null SetBoolean(u32 shader, cstring name, i8 value)
{
    glUniform1i(glGetUniformLocation(shader, name), (i32)value);
}

null SetInteger(u32 shader, cstring name, i32 value)
{
    glUniform1i(glGetUniformLocation(shader, name), value);
}

null SetFloat(u32 shader, cstring name, f32 value)
{
    glUniform1f(glGetUniformLocation(shader, name), value);
}
