#include "Window.h"
#include "Libraries.h"
#include "Logger.h"
#include <stdarg.h>

GLFWwindow* CreateKeyWindow(u16 width, u16 height, i32 x, i32 y, string title,
                            ...)
{
    // Get the variable arguments to be concatenated into the title.
    va_list args;
    va_start(args, title);
    // Allocate some space for the title string.
    char title_string[WINDOW_MAX_TITLE_LENGTH];

    // Try to concatenate the arguments into the string. If this fails, print
    // the error and return nothing.
    if (vsnprintf(title_string, WINDOW_MAX_TITLE_LENGTH, title, args) < 0)
    {
        PrintError("Failed to create title string for window, expected; '%s'. "
                   "Code: %d.",
                   title, errno);
        return NULL;
    }

    GLFWwindow* win;
#ifdef linux
    // Make GLFW create our window. Since, on GNOME (specifically Wayland),
    // window decorators are ugly as hell, I've decided to just have the window
    // be automatically in fullscreen borderless.
    win = glfwCreateWindow(width, height, title_string, glfwGetPrimaryMonitor(),
                           NULL);
#else
    // Make GLFW create our window.
    win = glfwCreateWindow(width, height, title_string, NULL, NULL);
#endif

    // If we fail to create the window, print the error and return nothing.
    if (win == NULL)
    {
        PrintGLFWError();
        return NULL;
    }

    // Make the window's OpenGL context the current one on this thread and print
    // our success.
    glfwMakeContextCurrent(win);
    PrintSuccess("Successfully created and made current the window '%s'.",
                 title_string);

    // Try to initialize GLAD. If this fails, the process kills itself, so we
    // don't bother checking for further errors.
    InitializeGLAD();

    // Return the address of the newly created window.
    return win;
}

null KillWindow(GLFWwindow* win)
{
    // Warn the user about the window's destruction.
    PrintWarning("Killed window '%s'.", glfwGetWindowTitle(win));
    glfwDestroyWindow(win);
}
