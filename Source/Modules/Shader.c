#include "Shader.h"
#include "Logger.h"

u8 _FileRead(FILE* file, char* buffer, i64 length)
{
    // Try to utilize fread and read the file's bytes into the given buffer. If
    // that fails, attempt to diagnose the error and print it. If we can't do
    // that, well, we're out of luck. Fail the process anyway.
    if (fread(buffer, 1, length, file) < length)
    {
        i32 err = ferror(file);
        if (err != 0)
        {
            PrintError("Failed to read a shader file. Code: %d.", err);
            return FAILURE;
        }
        PrintError("Ran into an error while reading a shader file, but "
                   "couldn't diagnose it.");
        return FAILURE;
    }

    // Since fread doesn't add a termination character, we've gotta ourselves.
    buffer[length] = '\0';
    return SUCCESS;
}
u8 _SetShaderSource(u32* shader, const char* source)
{
    // Try to set the source of the shader.
    glShaderSource(*shader, 1, &source, NULL);

    // If it fails, print the error and exit the method.
    if (!PrintGLError())
        return FAILURE;

    // Print that everything went swimingly.
    return SUCCESS;
}
u8 _GetCompilationError(u32 program, u8 type)
{
    i32 success_flag = 0;
    // Try to compile/link the shader. Get the status of the operation, and if
    // something's amiss, set the success flag to failure.
    if (type)
    {
        glCompileShader(program);
        glGetShaderiv(program, GL_COMPILE_STATUS, &success_flag);
    }
    else
    {
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success_flag);
    }

    if (!success_flag)
    {
        // Allocate enough space to store most (if not all) info logs generated
        // by shader problems.
        char info_log[512];
        // Get the info log, and copy it into the buffer, ready for us to print.
        if (type)
            glGetShaderInfoLog(program, 512, NULL, info_log);
        else
            glGetProgramInfoLog(program, 512, NULL, info_log);

        // Print what went wrong, and on what type of shader it happened.
        PrintError("There was an issue with the compilation of a shader (%d). "
                   "Log: '%s'.",
                   type, info_log);

        // Return that we failed :(.
        return FAILURE;
    }
    return SUCCESS;
}

u32 LoadShader(const char* name)
{
    // Set up the full shader paths, taking advantage of snprintf to concatenate
    // the full char* in one function call.
    char vertex_path[SHADER_PATH_MAX_LENGTH],
        fragment_path[SHADER_PATH_MAX_LENGTH];

    if (snprintf(vertex_path, SHADER_PATH_MAX_LENGTH,
                 "Assets/Shaders/%s/vertex.vs", name) < 0 ||
        snprintf(fragment_path, SHADER_PATH_MAX_LENGTH,
                 "Assets/Shaders/%s/fragment.fs", name) < 0)
    {
        PrintError(
            "Failed to construct a full shader path for the shader '%s'.",
            name);
        return FAILURE;
    }

    // Open the needed files in "read binary" mode, since we're going to just
    // directly rip the bytes and convince ourselves they're characters.
    FILE *vertex_file = fopen(vertex_path, "rb"),
         *fragment_file = fopen(fragment_path, "rb");

    // If the files opened, continue on. If not, print the error code and kill
    // the method.
    if (vertex_file && fragment_file)
    {
        // Attempt to set the file location pointer to the end of the file. If
        // that fails, print the error and kill the method.
        if (fseek(vertex_file, 0, SEEK_END) < 0 ||
            fseek(fragment_file, 0, SEEK_END) < 0)
        {
            PrintError("Failed to set a file pointer's location in a shader "
                       "file.. Code: %d.",
                       errno);
            return FAILURE;
        }

        // Use ftell to try and get the length of the file in bytes. We'll use
        // this for the buffer length later on.
        i64 vertex_length = ftell(vertex_file),
            fragment_length = ftell(fragment_file);
        if (vertex_length < 0 || fragment_length < 0)
        {
            PrintError("Failed to read the length of shader files. Code: %d.",
                       errno);
            return FAILURE;
        }

        // Use fseek to reset the file's location pointer to the beginning of
        // the buffer, as now we're going to read the file contents.
        if (fseek(vertex_file, 0, SEEK_SET) < 0 ||
            fseek(fragment_file, 0, SEEK_SET) < 0)
        {
            PrintError("Failed to reset the file location pointer's location "
                       "in a shader file. Code: %d.",
                       errno);
            return FAILURE;
        }

        // Create an array with enough characters to hold the file buffer (+ a
        // void termination character.).
        char vertex_buffer[vertex_length + 1],
            fragment_buffer[fragment_length + 1];

        // Attempt to read the files into memory. If that process fails, fail
        // this process.
        if (!_FileRead(vertex_file, vertex_buffer, vertex_length) ||
            !_FileRead(fragment_file, fragment_buffer, fragment_length))
            return FAILURE;

        // Close the shader files, as we don't need them anymore. We don't give
        // a damn if this fails.
        (void)fclose(vertex_file);
        (void)fclose(fragment_file);

        // Convert the strings into ones OpenGL will accept, and then initialize
        // the memory needed for the vertex and fragment shaders.
        const char *vertex_raw = vertex_buffer, *fragment_raw = fragment_buffer;
        u32 vertex = glCreateShader(GL_VERTEX_SHADER),
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
        // Set the source char* of the shaders, and fail if we can't.
        if (!_SetShaderSource(&vertex, vertex_raw) ||
            !_SetShaderSource(&fragment, fragment_raw))
            return FAILURE;

        // Try to compile each shader. If that fails, grab the error codes.
        if (!_GetCompilationError(vertex, 1) ||
            !_GetCompilationError(fragment, 1))
            return FAILURE;

        // Create the final program. This is basically just mashing the shaders
        // together in a special way so they work together in a pipeline.
        u32 final = glCreateProgram();
        // Attach each shader to the final program, ready to be compiled.
        glAttachShader(final, vertex);
        glAttachShader(final, fragment);
        // Try to compile the final program. If that fails, kill the function.
        if (!_GetCompilationError(final, 0))
            return FAILURE;

        // Delete the now useless individual shader programs.
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        // Gloat upon our success.
        PrintSuccess("Compiled the shader '%s' successfully.", name);
        return final;
    }

    PrintError("Failed to open vertex and/or fragment shader file for shader "
               "'%s'. Code: %d",
               name, errno);
    return FAILURE;
}

u8 UseShader(u32 shader)
{
    // Try to use the program. Afterward, run the PrintOpenGLError function,
    // which checks to see if there are any errors and prints them if so.
    glUseProgram(shader);
    if (!PrintGLError())
        return FAILURE;
    return SUCCESS;
}

void SetBoolean(u32 shader, const char* name, i8 value)
{
    glUniform1i(glGetUniformLocation(shader, name), (i32)value);
}

void SetInteger(u32 shader, const char* name, i32 value)
{
    glUniform1i(glGetUniformLocation(shader, name), value);
}

void SetFloat(u32 shader, const char* name, f32 value)
{
    glUniform1f(glGetUniformLocation(shader, name), value);
}
