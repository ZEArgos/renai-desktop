#include "Window.h"
#include "Libraries.h"
#include "Logger.h"
#include <math.h>

void _framebuffer_callback(GLFWwindow* window, i32 width, i32 height)
{
    // code graciously stolen + adapted from
    // https://diegomacario.github.io/2021/04/23/how-to-keep-the-aspect-ratio-of-an-opengl-window-constant.html

    i32 smallest_dimension = fmin(width, height), lower_left_corner_x = 0,
        lower_left_corner_y = 0;

    if (smallest_dimension == height)
        lower_left_corner_x = (width - smallest_dimension) / 2.0f;
    else lower_left_corner_y = (height - smallest_dimension) / 2.0f;

    glViewport(lower_left_corner_x, lower_left_corner_y, smallest_dimension,
               smallest_dimension);
    glScissor(lower_left_corner_x, lower_left_corner_y, smallest_dimension,
              smallest_dimension);
}

__CREATE_STRUCTURE(Window) CreateKeyWindow(i32 width, i32 height)
{
    Window* window = malloc(sizeof(Window));
    window->inner_window = glfwCreateWindow(width, height, TITLE, NULL, NULL);
    window->window_width = width;
    window->window_height = height;

    if (GetInnerWindow(window) == NULL)
    {
        PrintGLFWError();
        return NULL;
    }

    glfwSetFramebufferSizeCallback(GetInnerWindow(window),
                                   _framebuffer_callback);

    glfwMakeContextCurrent(GetInnerWindow(window));

#ifndef DEBUG_MODE
    glfwMaximizeWindow(win.inner_window);
#endif

    // PrintSuccess("Successfully created and made current the window '%s'.",
    //              TITLE);

    InitializeGLAD();

    _framebuffer_callback(GetInnerWindow(window), width, height);

    return window;
}

void SetWindowFullscreenState(Window* win, WindowFullscreenState state)
{
    switch (state)
    {
        case windowed:
            glfwSetWindowMonitor(win->inner_window, NULL, 0, 0,
                                 win->window_width, win->window_height, -1);
            glfwRestoreWindow(win->inner_window);
            return;
        case maximized:
            glfwSetWindowMonitor(win->inner_window, NULL, 0, 0,
                                 win->window_width, win->window_height, -1);
            glfwMaximizeWindow(win->inner_window);
            return;
        case borderless:
            glfwSetWindowMonitor(win->inner_window, glfwGetPrimaryMonitor(), 0,
                                 0, win->window_width, win->window_height, -1);
            return;
    }
}
