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

__CREATE_STRUCT(Window) CreateWindow(i32 width, i32 height, const char* caller)
{
    // Allocate the space for the window structure, failing the process if we
    // cannot, and printing our success if we can.
    Window* window = malloc(sizeof(Window));
    if (window == NULL)
        PrintError(
            "Failed to allocate the space for a window object. Code: %d.",
            errno);
    PrintSuccess(
        "Allocated memory for the main window of the application: %d bytes.",
        sizeof(Window));

    // Set the stored default dimensions for the windows. These are fallen back
    // upon when the window is reset from maximized borderless mode.
    window->window_width = width;
    window->window_height = height;

    // Create the underlying GLFW window of the application, using the
    // cross-platform tools available. If the window is not valid upon
    // completion of the function, print the error.
    window->inner_window = glfwCreateWindow(width, height, TITLE, NULL, NULL);
    if (!CheckWindowValidity(window)) PollGLFWErrors(__func__);
    PrintSuccess("Created the window with title '%s' successfully.", TITLE);

    // Make the OpenGL context of the window current, and initialize my OpenGL
    // wrangler of choice; GLAD.
    glfwMakeContextCurrent(GetInnerWindow(window));
    InitializeGLAD(__func__);

    // Set the framebuffer callback of the window, or the function that will be
    // called whenever the window is resized. Then, call the framebuffer
    // callback, as we want the clip box to appear off the bat immediately.
    glfwSetFramebufferSizeCallback(GetInnerWindow(window),
                                   _framebuffer_callback);
    _framebuffer_callback(GetInnerWindow(window), width, height);
    PrintSuccess("Set the framebuffer callback of the window.");

    // Return the allocated window to the caller.
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
