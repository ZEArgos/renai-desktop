#include "Window.h"
#include "Libraries.h"
#include "Logger.h"

GLFWwindow* GetInnerWindow(Window* win) { return win->inner_window; }

Window* CreateKeyWindow(u16 width, u16 height, i32 x, i32 y, string title)
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
    win->inner_window = glfwCreateWindow(width, height, win->title,
                                         glfwGetPrimaryMonitor(), NULL);
#else
    // Make GLFW create our window.
    win->inner_window = glfwCreateWindow(width, height, win->title, NULL, NULL);
#endif

    // If we fail to create the window, print the error and return nothing.
    if (win->inner_window == NULL)
    {
        win->initialized = 0;
        PrintGLFWError();
        return NULL;
    }

    // Make the window's OpenGL context the current one on this thread and print
    // our success.
    glfwMakeContextCurrent(win->inner_window);
    PrintSuccess("Successfully created and made current the window '%s'.",
                 title);

    // Try to initialize GLAD. If this fails, the process kills itself, so we
    // don't bother checking for further errors.
    InitializeGLAD();

    win->initialized = 1;
    // Return the address of the newly created window.
    return win;
}

null KillWindow(Window* win)
{
    // Warn the user about the window's destruction.
    PrintWarning("Killed window '%s'.", glfwGetWindowTitle(win->inner_window));
    glfwDestroyWindow(win->inner_window);
    free(win);
}
