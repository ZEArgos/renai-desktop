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
    char title[WINDOW_MAX_TITLE_LENGTH];
} Window;

/**
 * @brief Get the GLFWwindow interface inside the given Window. We do some
 * checks inside this function to make sure we're not gonna acess nonexistant
 * data, or at least warn the user when we do.
 * @param win A pointer to the window we're trying to check.
 * @return A pointer to the inner GLFW interface of the window.
 */
GLFWwindow* GetInnerWindow(Window* win);

/**
 * @brief This function is rather strange. I didn't want to add another
 * dependency onto this project, especially not one I had no idea how to use,
 * and I would've needed to in order to decorate the window on Linux. However,
 * the decorators are gone when fully maximized, so I just decided that on
 * Linux, this function creates a borderless maximized window, and on Windows,
 * just creates a normal one. Because of this, the @param title, @param x, and
 * @param y arguments do nothing visible on Linux.
 * @param x The X coordinate of the window.
 * @param y The Y coordinate of the window.
 * @param title The given title of the window.
 * @return A pointer to the created window.
 */
Window* CreateKeyWindow(i32 x, i32 y, string title);

/**
 * @brief Kill the window passed in. This is a message-logging wrapper around
 * the @ref glfwDestroyWindow function. This will set the given Window to NULL.
 * @param win The window to destroy.
 */
null KillWindow(Window* win);

#endif // _RENAI_WINDOW_
