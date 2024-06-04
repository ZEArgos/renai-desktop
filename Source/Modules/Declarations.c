#include "Declarations.h" // The Declarations mother header. This provides all the function declarations.
#include "Logger.h" // The logger header, for use in logging output codes.
#include <time.h>   // The system header for geting the current time and date.

u8 PrintGLFWError(void)
{
    // Get both the error code and human-readable description of the error.
    const char* description;
    i32 code = glfwGetError(&description);

    // If the code was not nothing, print the error and return failure.
    if (code != 0)
    {
        // Try to print the message to console.
        PrintError("An error happened with GLFW. Code: %d. Message: '%s'.",
                   code, description);
        return FAILURE;
    }
    return SUCCESS;
}

u8 PrintGLError(void)
{
    // Try to get the error code, and if we find one, print it and return
    // failure.
    i32 err = glGetError();
    if (err != 0)
    {
        PrintError("Ran into an error with OpenGL. Code: %d", err);
        return FAILURE;
    }
    // Return that nothing has yet gone amiss.
    return SUCCESS;
}

void GetDateString(char* buffer)
{
    // Get the raw time, and then the full, local time as reported by the user's
    // computer.
    time_t time_raw = time(NULL);
    struct tm time_full = *localtime(&time_raw);

    // Print the raw time into the buffer, with a maxlength of 64 characters.
    // Note that we do some seemingly-strange addition with some of the
    // variables, but that's simply because we're reformatting some date values.
    snprintf(buffer, 64, "%d-%02d-%02d at %02d:%02d:%02d",
             time_full.tm_year + 1900, time_full.tm_mon + 1, time_full.tm_mday,
             time_full.tm_hour, time_full.tm_min, time_full.tm_sec);
}
