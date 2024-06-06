/**
 * @file Window.h
 * @author Zenais Argos
 * @brief A file to provide the various functions and data structures needed for
 * Renai's window management system.
 * @date 2024-05-24
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_WINDOW_
#define _RENAI_WINDOW_

// Include the declarations file so I can use my small utility functions and
// typedefs.
#include "Declarations.h"

/**
 * @brief The max possible length of a window's title text. This is put in place
 * to prevent certain overflow errors.
 */
#define WINDOW_MAX_TITLE_LENGTH 128

/**
 * @brief A data structure to contain all the information about a specific
 * window. Minimal interaction should be done with these objects, instead let
 * the given functions interface with them.
 */
typedef struct Window
{
    /**
     * @brief The inner GLFW interface of the window. This is what is actually
     * displayed on screen.
     */
    GLFWwindow* inner_window;
    /**
     * @brief The window's title value. This can only be 127 characters long (+
     * a \0 character.)
     */
    char* title;
    i32 window_width;
    i32 window_height;
} Window;

/**
 * @brief The ratio of width to height in the application's graphical rendering.
 */
#define APPLICATION_ASPECT_RATIO 1

/**
 * @brief Get the GLFWwindow interface inside the given Window. We do some
 * checks inside this function to make sure we're not gonna acess nonexistant
 * data, or at least warn the user when we do.
 * @param win A pointer to the window we're trying to check.
 * @return A pointer to the inner GLFW interface of the window.
 */
GLFWwindow* GetInnerWindow(Window* win);

Window CreateKeyWindow(void);

/**
 * @brief Kill the window passed in. This is a message-logging wrapper around
 * the @ref glfwDestroyWindow function. This will set the given Window to NULL.
 * @param win The window to destroy.
 */
void KillWindow(Window* win);

#endif // _RENAI_WINDOW_
