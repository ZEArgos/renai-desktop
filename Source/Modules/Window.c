#include "Window.h"
#include "Libraries.h"
#include "Logger.h"
#include "Renderer.h"
#include <math.h>

/**
 * @brief The internal application structure. This is defined in Application.h,
 * so we just define it as external here and call it a day.
 */
extern struct
{
    u8 initialized;
    f32 screen_width, screen_height;
    Window window;
    Renderer renderer;
} _application;

void _framebuffer_callback(GLFWwindow* win, i32 width, i32 height)
{
    // code graciously stolen + adapted from
    // https://diegomacario.github.io/2021/04/23/how-to-keep-the-aspect-ratio-of-an-opengl-window-constant.html

    i32 smallest_dimension = fmin(width, height), lower_left_corner_x = 0,
        lower_left_corner_y = 0;

    if (smallest_dimension == height)
        lower_left_corner_x = (width - smallest_dimension) / 2.0f;
    else
        lower_left_corner_y = (height - smallest_dimension) / 2.0f;

    glViewport(lower_left_corner_x, lower_left_corner_y, smallest_dimension,
               smallest_dimension);
    glScissor(lower_left_corner_x, lower_left_corner_y, smallest_dimension,
              smallest_dimension);
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

Window* CreateKeyWindow(void)
{
    Window* win = malloc(sizeof(struct Window));

    // Get the primary monitor and its video information.
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* res = glfwGetVideoMode(primary);
    win->window_width = (i32)(res->width / 1.25);
    win->window_height = (i32)(res->height / 1.25);

    // Make GLFW create our window.
    win->inner_window = glfwCreateWindow(win->window_width, win->window_height,
                                         TITLE, NULL, NULL);

    // If we fail to create the window, print the error and return nothing.
    if (win->inner_window == NULL)
    {
        PrintGLFWError();

        // Make sure we don't spring a memory leak.
        free(win);
        return NULL;
    }

    glfwSetFramebufferSizeCallback(win->inner_window, _framebuffer_callback);

    // Make the window's OpenGL context the current one on this thread.
    glfwMakeContextCurrent(win->inner_window);

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

    _framebuffer_callback(win->inner_window, win->window_width,
                          win->window_height);

    win->title = TITLE;
    // Return the address of the newly created window.
    return win;
}

void KillWindow(Window* win)
{
    glfwDestroyWindow(win->inner_window);
    // Warn the user about the window's destruction.
    PrintWarning("Killed window '%s'.", win->title);
    free(win);
}
