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
    f32 aspect_ratio;
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
    {
        f32 vertical_bar_width = width - smallest_dimension;
        lower_left_corner_x = vertical_bar_width / 2.0f;
        lower_left_corner_y = 0;
    }
    else
    {
        f32 horizontal_bar_height = height - smallest_dimension;
        lower_left_corner_y = horizontal_bar_height / 2.0f;
        lower_left_corner_x = 0;
    }

    glViewport(lower_left_corner_x, lower_left_corner_y, smallest_dimension,
               smallest_dimension);
    glScissor(lower_left_corner_x, lower_left_corner_y, smallest_dimension,
              smallest_dimension);
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
    i32 window_width = (i32)(res->width / 1.25),
        window_height = (i32)(res->height / 1.25);

    // Make GLFW create our window.
    GLFWwindow* temporary_win =
        glfwCreateWindow(window_width, window_height, TITLE, NULL, NULL);

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

    _framebuffer_callback(temporary_win, window_width, window_height);

    // Return the address of the newly created window.
    return (struct Window){temporary_win, TITLE};
}

void KillWindow(Window* win)
{
    glfwDestroyWindow(win->inner_window);
    // Warn the user about the window's destruction.
    PrintWarning("Killed window '%s'.", win->title);
}
