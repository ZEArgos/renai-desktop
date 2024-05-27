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
 * @brief This function is rather strange. I didn't want to add another
 * dependency onto this project, especially not one I had no idea how to use,
 * and I would've needed to in order to decorate the window on Linux. However,
 * the decorators are gone when fully maximized, so I just decided that on
 * Linux, this function creates a borderless maximized window, and on Windows,
 * just creates a normal one. Because of this, the @param title, @param width,
 * @param height, @param x, and @param y arguments do nothing visible on Linux.
 * @param width The width of the window in screen coordinates.
 * @param height The height of the window in screen coordinates.
 * @param x The X coordinate of the window.
 * @param y The Y coordinate of the window.
 * @param title The given title of the window.
 * @return A pointer to the created window.
 */
GLFWwindow* CreateKeyWindow(u16 width, u16 height, i32 x, i32 y, string title);

/**
 * @brief Kill the window passed in. This is a message-logging wrapper around
 * the @ref glfwDestroyWindow function.
 * @param win The window to destroy.
 */
null KillWindow(GLFWwindow* win);

#endif // _RENAI_WINDOW_