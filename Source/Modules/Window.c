#include "Window.h"
#include "Libraries.h"
#include "Logger.h"
#include <stdarg.h>

#if defined(linux)

GLFWwindow* CreateWindow(u16 width, u16 height, i32 x, i32 y, string title)
{
    GLFWwindow* win =
        glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
    if (win == NULL)
    {
        PrintGLFWError();
        return NULL;
    }

    glfwMakeContextCurrent(win);
    PrintSuccess("Successfully created and make current the window '%s'.",
                 title);

    return win;
}

#else

GLFWwindow* CreateWindow(u16 width, u16 height, i32 x, i32 y, string title)
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
    if (win == NULL)
        exit(-1);
    InitializeGLAD();

    return win;
}

null KillWindow(GLFWwindow* win)
{
    PrintWarning("Killed window '%s'.", glfwGetWindowTitle(win));
    glfwDestroyWindow(win);
}
