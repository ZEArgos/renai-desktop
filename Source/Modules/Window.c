#include "Window.h"
#include "Logger.h"
#include <stdarg.h>

null _PrintGLFWError(void)
{
    cstring description;
    i32 code = glfwGetError(&description);

    if (PrintError("An error happened with GLFW. Code: %d. Message: '%s'.",
                   code, description) == FAILURE)
        exit(-1);
}

u8 InitializeGLFW(void)
{
    if (!glfwInit())
    {
        _PrintGLFWError();
        return FAILURE;
    }
    PrintSuccess("Initialized GLFW successfully. Version: '%s'.",
                 glfwGetVersionString());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    PrintSuccess("Set GLFW window hints successfully.");
    return SUCCESS;
}
null KillGLFW(void)
{
    glfwTerminate();
    PrintWarning("Terminated GLFW.");
}
u8 InitializeGLAD(void)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        PrintError(
            "Failed to initialize GLAD. Please restart the application.");
        return FAILURE;
    }

    return SUCCESS;
}

#if defined(linux)

GLFWwindow* CreateWindow(u16 width, u16 height, i32 x, i32 y, string title, ...)
{
    GLFWwindow* win =
        glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
    if (win == NULL)
    {
        _PrintGLFWError();
        return NULL;
    }
    glfwMakeContextCurrent(win);

    return win;
}

#else

GLFWwindow* CreateWindow(string title, u16 width, u16 height, i32 x, i32 y)
{
    GLFWwindow* win = glfwCreateWindow(width, height, title, NULL, NULL);
    if (win == NULL)
    {
        _PrintGLFWError();
        return NULL;
    }
    glfwSetWindowPos(win, x, y);
    glfwMakeContextCurrent(win);

    return win;
}

#endif

GLFWwindow* CreateKeyWindow(u16 width, u16 height, i32 x, i32 y, string title,
                            ...)
{
    va_list args;
    va_start(args, title);

    char title_string[WINDOW_MAX_TITLE_LENGTH];

    //!!!! error checking
    vsnprintf(title_string, WINDOW_MAX_TITLE_LENGTH, title, args);

    GLFWwindow* win = CreateWindow(width, height, x, y, title_string);
    InitializeGLAD();
    return win;
}

null DestroyWindow(GLFWwindow* win) { glfwDestroyWindow(win); }
