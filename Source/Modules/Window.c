#include "Window.h"
#include "Libraries.h"
#include "Logger.h"

void _framebuffer_callback(GLFWwindow* win, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}

u8 CheckWindowValidity(Window* win)
{
    if (win->inner_window == NULL || win->title == NULL)
        return FAILURE;
    return SUCCESS;
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

Window CreateKeyWindow(void)
{
    // Get the primary monitor and its video information.
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* res = glfwGetVideoMode(primary);
    // Make GLFW create our window.
    GLFWwindow* temporary_win = glfwCreateWindow(
        (i32)(res->width / 1.25), (i32)(res->height / 1.25), TITLE, NULL, NULL);

    // If we fail to create the window, print the error and return nothing.
    if (temporary_win == NULL)
    {
        PrintGLFWError();

        // Make sure we don't spring a memory leak.
        free(temporary_win);
        return (struct Window){NULL};
    }

    glfwSetFramebufferSizeCallback(temporary_win, _framebuffer_callback);

    // Make the window's OpenGL context the current one on this thread.
    glfwMakeContextCurrent(temporary_win);

#ifndef DEBUG_MODE
    // If we're not in debug mode, maximize the created window.
    glfwMaximizeWindow(win->inner_window);
#endif

    // Finally, print our success.
    PrintSuccess("Successfully created and made current the window '%s'.",
                 TITLE);

    // Try to initialize GLAD. If this fails, the process kills itself, so we
    // don't bother checking for further errors.
    InitializeGLAD();

    // Return the address of the newly created window.
    return (struct Window){temporary_win, TITLE};
}

void KillWindow(Window* win)
{
    glfwDestroyWindow(win->inner_window);
    // Warn the user about the window's destruction.
    PrintWarning("Killed window '%s'.", win->title);
}
