#include "Declarations.h"
#include <sys/time.h>
#include <unistd.h>

#include "Logger.h"
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
    i32 err = glGetError();
    if (err != 0)
    {
        PrintError("Ran into an error with OpenGL. Code: %d", err);
        return FAILURE;
    }
    return SUCCESS;
}

#include <time.h>
void GetDateString(char* buffer)
{
    time_t time_raw = time(NULL);
    struct tm time_full = *localtime(&time_raw);

    snprintf(buffer, 128, "%d-%02d-%02d at %02d:%02d:%02d",
             time_full.tm_year + 1900, time_full.tm_mon + 1, time_full.tm_mday,
             time_full.tm_hour, time_full.tm_min, time_full.tm_sec);
}
