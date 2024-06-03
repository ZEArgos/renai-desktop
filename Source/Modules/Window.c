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

Window* CreateKeyWindow(i32 x, i32 y, string title)
{
    // Allocate enough space for the window.
    Window* win = malloc(sizeof(Window));
    // Give the window's title some text. To prevent buffer overflows, this is
    // strictly cut off at the 127th index. We cast this to void because we
    // don't give a fuck about this return value.
    (void)strncpy(win->title, title, WINDOW_MAX_TITLE_LENGTH);

#ifdef linux
    // Make GLFW create our window. Since, on GNOME (specifically Wayland),
    // window decorators are ugly as hell, I've decided to just have the
    // window be automatically in fullscreen borderless.
    // win->inner_window = glfwCreateWindow(1, 1, win->title, NULL, NULL);
#else
    // Get the primary monitor and its video information.
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* res = glfwGetVideoMode(primary);
    // Make GLFW create our window.
    win->inner_window =
        glfwCreateWindow((i32)(res->width / 1.25), (i32)(res->height / 1.25),
                         win->title, NULL, NULL);
#endif

    // Get the primary monitor and its video information.
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* res = glfwGetVideoMode(primary);
    // Make GLFW create our window.
    win->inner_window =
        glfwCreateWindow((i32)(res->width / 1.25), (i32)(res->height / 1.25),
                         win->title, NULL, NULL);

    // If we fail to create the window, print the error and return nothing.
    if (win->inner_window == NULL)
    {
        PrintGLFWError();

        // Make sure we don't spring a memory leak.
        free(win);
        return NULL;
    }

#ifdef linux
    // Set the window to borderless fullscreen.
    // glfwSetWindowMonitor(win->inner_window, glfwGetPrimaryMonitor(), 0, 0, 1,
    // 1,
    //                      0);
#endif

    // Set the position of the window as given.
    glfwSetWindowPos(win->inner_window, x, y);
    glfwSetFramebufferSizeCallback(win->inner_window, _framebuffer_callback);

    // Make the window's OpenGL context the current one on this thread and
    // print our success.
    glfwMakeContextCurrent(win->inner_window);
    PrintSuccess("Successfully created and made current the window '%s'.",
                 win->title);

    // Try to initialize GLAD. If this fails, the process kills itself, so we
    // don't bother checking for further errors.
    InitializeGLAD();

    // Return the address of the newly created window.
    return win;
}

null KillWindow(Window* win)
{
    glfwDestroyWindow(win->inner_window);
    // Warn the user about the window's destruction.
    PrintWarning("Killed window '%s'.", win->title);
    // Free the window's associated memory space.
    free(win);
    win = NULL;
}
