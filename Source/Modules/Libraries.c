#include "Libraries.h"
#include "Logger.h"

null PrintGLFWError(void)
{
    cstring description;
    i32 code = glfwGetError(&description);

    if (PrintError("An error happened with GLFW. Code: %d. Message: '%s'.",
                   code, description) == FAILURE)
        exit(-1);
}

null InitializeGLFW(void)
{
    if (!glfwInit())
    {
        PrintGLFWError();
        exit(-1);
    }
    PrintSuccess("Initialized GLFW successfully. Version: '%s'.",
                 glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    PrintSuccess("Set GLFW window hints successfully.");
}
null KillGLFW(void)
{
    glfwTerminate();
    PrintWarning("Terminated GLFW.");
}
null InitializeGLAD(void)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        PrintError("Failed to initialize GLAD. Code: %d.", glGetError());
        exit(-1);
    }
    PrintSuccess("Initialized GLAD and loaded OpenGL. Version: '%s'.",
                 glGetString(GL_VERSION));
}
