#include "Window.h"
#include "Libraries.h"
#include "Logger.h"

void _framebuffer_callback(GLFWwindow* win, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* GetInnerWindow(Window* win)
{
    // Check to make sure the data we're returning isn't bullshit, and if it is,
    // warn the user.
    if (win == NULL || win->inner_window == NULL)
    {
        PrintWarning(
            "Tried to access window interface before it was initialized.");
        return NULL;
    }

    // Return the value we want, the inner GLFW interface.
    return win->inner_window;
}

Window CreateKeyWindow(i32 x, i32 y, string title)
{
    // Get the primary monitor and its video information.
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* res = glfwGetVideoMode(primary);
    // Make GLFW create our window.
    GLFWwindow* temporary_win = glfwCreateWindow(
        (i32)(res->width / 1.25), (i32)(res->height / 1.25), title, NULL, NULL);

    // If we fail to create the window, print the error and return nothing.
    if (temporary_win == NULL)
    {
        PrintGLFWError();

        // Make sure we don't spring a memory leak.
        free(temporary_win);
        return (struct Window){NULL};
    }

    // Set the position of the window as given.
    glfwSetWindowPos(temporary_win, x, y);
    glfwSetFramebufferSizeCallback(temporary_win, _framebuffer_callback);

    // Make the window's OpenGL context the current one on this thread.
    glfwMakeContextCurrent(temporary_win);

#ifndef DEBUG_MODE
    // If we're not in debug mode, maximize the created window.
    glfwMaximizeWindow(win->inner_window);
#endif

    // Finally, print our success.
    PrintSuccess("Successfully created and made current the window '%s'.",
                 title);

    // Try to initialize GLAD. If this fails, the process kills itself, so we
    // don't bother checking for further errors.
    InitializeGLAD();

    // Return the address of the newly created window.
    return (struct Window){temporary_win, title};
}

null KillWindow(Window* win)
{
    glfwDestroyWindow(win->inner_window);
    // Warn the user about the window's destruction.
    PrintWarning("Killed window '%s'.", win->title);
}
