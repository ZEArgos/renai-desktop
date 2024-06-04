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
    float desiredAspectRatio = 1;

    int widthOfViewport = 0, heightOfViewport = 0;
    // These are two new values that we will be calculating in this function
    int lowerLeftCornerOfViewportX = 0, lowerLeftCornerOfViewportY = 0;

    float requiredHeightOfViewport = width * (1.0f / desiredAspectRatio);
    if (requiredHeightOfViewport > height)
    {
        float requiredWidthOfViewport = height * desiredAspectRatio;
        if (requiredWidthOfViewport > width)
        {
            // std::cout << "Error: Couldn't find dimensions that preserve the
            // aspect ratio\n";
            exit(-1);
        }
        else
        {
            // Remember that if we reach this point you will observe vertical
            // bars on the left and right
            widthOfViewport = requiredWidthOfViewport;
            heightOfViewport = height;

            // The widths of the two vertical bars added together are equal to
            // the difference between the width of the framebuffer and the width
            // of the viewport
            float widthOfTheTwoVerticalBars = width - widthOfViewport;

            // Set the X position of the lower left corner of the viewport equal
            // to the width of one of the vertical bars. By doing this, we
            // center the viewport horizontally and we make vertical bars appear
            // on the left and right
            lowerLeftCornerOfViewportX = widthOfTheTwoVerticalBars / 2.0f;
            // We don't need to center the viewport vertically because we are
            // using the height of the framebuffer as the height of the viewport
            lowerLeftCornerOfViewportY = 0;
        }
    }
    else
    {
        // Remember that if we reach this point you will observe horizontal bars
        // on the top and bottom
        widthOfViewport = width;
        heightOfViewport = requiredHeightOfViewport;

        // The heights of the two horizontal bars added together are equal to
        // the difference between the height of the framebuffer and the height
        // of the viewport
        float heightOfTheTwoHorizontalBars = height - heightOfViewport;

        // We don't need to center the viewport horizontally because we are
        // using the width of the framebuffer as the width of the viewport
        lowerLeftCornerOfViewportX = 0;
        // Set the Y position of the lower left corner of the viewport equal to
        // the height of one of the vertical bars. By doing this, we center the
        // viewport vertically and we make horizontal bars appear on the top and
        // bottom
        lowerLeftCornerOfViewportY = heightOfTheTwoHorizontalBars / 2.0f;
    }

    // Call glViewport to specify the new drawing area
    // By specifying its lower left corner, we center it
    glViewport(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY,
               widthOfViewport, heightOfViewport);
    glScissor(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY,
              widthOfViewport, heightOfViewport);
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
