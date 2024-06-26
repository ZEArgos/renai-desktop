#include "Window.h"
#include <Libraries.h>
#include <math.h>

/**
 * @brief The callback triggered by GLFW when the framebuffer's (image buffer)
 * size changes.
 * @param window The window whose size changed. (We don't use this variable.)
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
void _FramebufferCallback(GLFWwindow* window, i32 width, i32 height)
{
    // Figure out which side we're going to be clamping the aspect ratio on.
    i32 smallest_dimension = fmin(width, height), lower_left_corner_x = 0,
        lower_left_corner_y = 0;

    if (smallest_dimension == height)
        lower_left_corner_x = (width - smallest_dimension) / 2.0f;
    else lower_left_corner_y = (height - smallest_dimension) / 2.0f;

    // Set both the OpenGL viewport and scissor box.
    SetOpenGLViewport(lower_left_corner_x, lower_left_corner_y,
                      smallest_dimension, smallest_dimension);
}

__CREATE_STRUCT_KILLFAIL(Window)
CreateWindow(i32 width, i32 height, const char* caller)
{
    Window* window = malloc(sizeof(Window));
    if (window == NULL)
        PrintError(
            "Failed to allocate the space for a window object. Code: %d.",
            errno);
    PrintSuccess(
        "Allocated memory for the main window of the application: %d bytes.",
        sizeof(Window));

    window->window_width = width;
    window->window_height = height;

    // Create the window with the given width, height, a title of whatever TITLE
    // is defined as, bordered, and primary-monitored.
    window->inner_window = glfwCreateWindow(width, height, TITLE, NULL, NULL);
    if (GetInnerWindow(window) == NULL) PollGLFWErrors(__func__);
    PrintSuccess("Created the window with title '%s' successfully.", TITLE);

    glfwMakeContextCurrent(GetInnerWindow(window));
    InitializeGLAD(__func__);

    // Call the new framebuffer callback as to set the scissor box correctly.
    glfwSetFramebufferSizeCallback(GetInnerWindow(window),
                                   _FramebufferCallback);
    _FramebufferCallback(GetInnerWindow(window), width, height);
    PrintSuccess("Set the framebuffer callback of the window.");

    return window;
}

void SetWindowFullscreenState(Window* window, WindowFullscreenState state)
{
    switch (state)
    {
        case windowed:
            // We unset the window monitor both here and on the "maximized" case
            // simply because I believe it provides a more polished experience
            // when we can switch between states seamlessly; without this call
            // the window wouldn't become maximized/restored until borderless
            // was unset.
            UnsetWindowMonitor(window);
            glfwRestoreWindow(GetInnerWindow(window));
            return;
        case maximized:
            UnsetWindowMonitor(window);
            glfwMaximizeWindow(GetInnerWindow(window));
            return;
        case borderless: SetWindowMonitor(window); return;
    }
}

void ToggleWindowFullscreenState(Window* window, WindowFullscreenState state)
{
    // A little blocky, but it's far more efficient to do this as an if chain
    // than a switch.
    if (state == windowed || state == maximized)
        SetWindowFullscreenState(
            window,
            !glfwGetWindowAttrib(GetInnerWindow(window), GLFW_MAXIMIZED));
    else if (glfwGetWindowMonitor(GetInnerWindow(window)) == NULL)
        SetWindowFullscreenState(window, borderless);
    else SetWindowFullscreenState(window, maximized);
}
