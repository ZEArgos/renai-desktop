#include "Libraries.h"
#include <Logger.h>

__KILLFAIL InitializeGLFW(void)
{
    // Try to initialize GLFW, and if it fails, kill the process.
    if (!glfwInit()) PollGLFWErrors();

    // Set all the needed window hints to tell GLFW what version of
    // OpenGL we're trying to take advantage of.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    PrintSuccess("Initialized GLFW successfully. Version: %.5s.",
                 glfwGetVersionString());
}

void InitializeGLAD(void)
{
    // Using the built-in GLAD initializer, load OpenGL using GLFW's
    // procedure address. If this fails, kill the application.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        PollOpenGLErrors();

    // Set the OpenGL hint to indicate that we will be using various
    // depth-based functions.
    glEnable(GL_DEPTH_TEST);

    PrintSuccess(
        "Initialized GLAD and loaded OpenGL. Version: %.18s.",
        glGetString(GL_VERSION));
}
