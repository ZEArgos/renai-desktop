#include "Libraries.h"
#include "Logger.h"

null PrintGLFWError(void)
{
    // Get both the error code and human-readable description of the error.
    cstring description;
    i32 code = glfwGetError(&description);

    // Try to print the message to console.
    PrintError("An error happened with GLFW. Code: %d. Message: '%s'.", code,
               description);
}

null InitializeGLFW(void)
{
    // Try to initialize GLFW, and if it fails, kill the process. If it doesn't,
    // print success to the console, along with the GLFW version we're working
    // with.
    if (!glfwInit())
    {
        PrintGLFWError();
        exit(-1);
    }
    PrintSuccess("Initialized GLFW successfully. Version: '%s'.",
                 glfwGetVersionString());

    // Set all the needed window hints to tell GLFW what version of OpenGL we're
    // trying to take advantage of.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    PrintSuccess("Set GLFW window hints successfully.");
}

null KillGLFW(void)
{
    // Kill GLFW and warn of the execution.
    glfwTerminate();
    PrintWarning("Terminated GLFW.");
}

null InitializeGLAD(void)
{
    // Using the built-in GLAD initializer, load OpenGL using GLFW's procedure
    // address. If this fails, kill the application.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        PrintError("Failed to initialize GLAD. Code: %d.", glGetError());
        exit(-1);
    }
    PrintSuccess("Initialized GLAD and loaded OpenGL. Version: '%s'.",
                 glGetString(GL_VERSION));
}
