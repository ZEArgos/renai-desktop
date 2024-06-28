/**
 * @file Window.h
 * @author Zenais Argos
 * @brief A file to provide the various functions and data structures
 * needed for Renai's window management system.
 * @date 2024-05-24
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_WINDOW_
#define _RENAI_WINDOW_

// The type definitions in this file are what we're after.
#include <Declarations.h>
// Provides the functionality to log things to the standard output /
// error medium.
#include <Logger.h>

/**
 * @brief A wrapper for GLFW's @ref GLFWwindow with some added
 * information for ease of information.
 */
typedef struct Window
{
    /**
     * @brief The inner, wrapped GLFW window. This does the heavy
     * lifting.
     */
    GLFWwindow* inner_window;
    /**
     * @brief The width of the window when it was first created, used
     * for restoration purposes.
     */
    i32 window_width;
    /**
     * @brief The height of the window when it was first created, used
     * for restoration purposes.
     */
    i32 window_height;
} Window;

/**
 * @brief An enum to describe the possible states of fullscreen that a
 * window can be in (windowed, maximized, or borderless).
 */
typedef enum WindowFullscreenState
{
    windowed,
    maximized,
    borderless
} WindowFullscreenState;

/**
 * @brief Create a new window, and set up some of the various
 * under-the-hood bits of it.
 * @param width The requested width of the window.
 * @param height The requested height of the window.
 * @return A pointer to the created window.
 */
__CREATE_STRUCT_KILLFAIL(Window)
CreateWindow(i32 width, i32 height);

/**
 * @brief This is a simple getter to grab the inner window of a Window
 * object. It is basically just a replacement for @ref
 * window->inner_window.
 * @param window The window to get from.
 */
__INLINE __GET_STRUCT(GLFWwindow) GetInnerWindow(Window* window)
{
    return window->inner_window;
}

/**
 * @brief Get a flag telling the application if the given window
 * should close or not.
 * @param window The window to check.
 * @return A boolean expression, false for the window should continue,
 * true for the window should close.
 */
__INLINE __BOOLEAN GetWindowShouldClose(Window* window)
{
    return glfwWindowShouldClose(GetInnerWindow(window));
}

/**
 * @brief Close the given window.
 * @param window The window to close.
 */
__INLINE void CloseWindow(Window* window)
{
    glfwSetWindowShouldClose(window->inner_window, 1);
}

/**
 * @brief Set the monitor of the given window, using its stored size
 * and the primary monitor.
 * @param window The window to set.
 */
__INLINE void SetWindowMonitor(Window* window)
{
    glfwSetWindowMonitor(
        window->inner_window, glfwGetPrimaryMonitor(), 0, 0,
        window->window_width, window->window_height, -1);
}

/**
 * @brief Remove the monitor of the given window, sending it back to
 * bordered mode.
 * @param window The window to set.
 */
__INLINE void UnsetWindowMonitor(Window* window)
{
    glfwSetWindowMonitor(window->inner_window, NULL, 0, 0,
                         window->window_width, window->window_height,
                         -1);
}

/**
 * @brief Free the given window's storage and terminate GLFW.
 * @param window The window to destroy.
 */
__INLINE void KillWindow(Window* window)
{
    glfwDestroyWindow(window->inner_window);
    free(window);
    PrintWarning("Killed window '%s'.", TITLE);
    // We assume that GLFW is no longer needed after this call.
    glfwTerminate();
}

/**
 * @brief Set the given window's fullscreen state to the given value.
 * @param window The window to set.
 * @param state The state to use.
 */
void SetWindowFullscreenState(Window* window,
                              WindowFullscreenState state);

/**
 * @brief Similarly to @ref SetWindowFullscreenState, this function
 * affects the given window's fullscreen state, but instead of setting
 * it the method just negates the current value.
 * @param window The window whose attributes to toggle.
 * @param state The state to toggle.
 */
void ToggleWindowFullscreenState(Window* window,
                                 WindowFullscreenState state);

/**
 * @brief Restore the window to a normal, bordered state of whatever
 * size the window was orginally set to.
 */
#define RestoreWindow(window)                                        \
    SetWindowFullscreenState(window, windowed)

/**
 * @brief Sister function to @ref RestoreWindow, toggles the states
 * instead of sets it.
 */
#define ToggleRestoreWindow(window)                                  \
    ToggleWindowFullscreenState(window, windowed)

/**
 * @brief Maximize the window, but keep the border.
 */
#define MaximizeWindow(window)                                       \
    SetWindowFullscreenState(window, maximized)

/**
 * @brief Sister function to @ref MaximizeWindow, toggles the states
 * instead of sets it.
 */
#define ToggleMaximizeWindow(window)                                 \
    ToggleWindowFullscreenState(window, maximized)

/**
 * @brief Set the window to a fullscreen, borderless state.
 */
#define FullscreenWindow(window)                                     \
    SetWindowFullscreenState(window, borderless)

/**
 * @brief Sister function to @ref FullscreenWindow, toggles the states
 * instead of sets it.
 */
#define ToggleFullscreenWindow(window)                               \
    ToggleWindowFullscreenState(window, borderless)

#endif // _RENAI_WINDOW_
