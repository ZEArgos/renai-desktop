#include "Libraries.h" // The Libraries mother header, provides the function declarations defined here.
#include "Declarations.h" // Provides the various type definitions, includes, and methods needed in this translation unit.
#include "Logger.h" // Provides the functionality needed to log values to the terminal.

void InitializeGLFW(void)
{
    // Try to initialize GLFW, and if it fails, kill the process. If it doesn't,
    // print success to the console, along with the GLFW version we're working
    // with.
    glfwInit();
    if (!PrintGLFWError()) exit(-1);

    // Set all the needed window hints to tell GLFW what version of OpenGL we're
    // trying to take advantage of.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    PrintSuccess("Initialized GLFW successfully. Version: '%s'.",
                 glfwGetVersionString());
}

void KillGLFW(void)
{
    // Kill GLFW and warn of the execution.
    glfwTerminate();
    PrintWarning("Terminated GLFW.");
}

void InitializeGLAD(void)
{
    // Using the built-in GLAD initializer, load OpenGL using GLFW's procedure
    // address. If this fails, kill the application.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        PrintError("Failed to initialize GLAD. Code: %d.", glGetError());
        exit(-1);
    }
    // Set the OpenGL hint to indicate that we will be using various depth-based
    // functions.
    glEnable(GL_DEPTH_TEST);

    PrintSuccess("Initialized GLAD and loaded OpenGL. Version: '%s'.",
                 glGetString(GL_VERSION));
}
