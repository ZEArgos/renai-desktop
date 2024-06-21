#include "Libraries.h" // The Libraries mother header, provides the function declarations defined here.
#include "Declarations.h" // Provides the various type definitions, includes, and methods needed in this translation unit.
#include "Logger.h" // Provides the functionality needed to log values to the terminal.

__KILLFAIL InitializeGLFW(void)
{
    // Try to initialize GLFW, and if it fails, kill the process.
    if (!glfwInit()) PollGLFWErrors(__func__);

    // Set all the needed window hints to tell GLFW what version of OpenGL we're
    // trying to take advantage of.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Print our success to the standard output.
    PrintSuccess("Initialized GLFW successfully. Version: %.5s.",
                 glfwGetVersionString());
}

void InitializeGLAD(const char* caller)
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

    // PrintSuccess("Initialized GLAD and loaded OpenGL. Version: '%s'.",
    //              glGetString(GL_VERSION));
}
