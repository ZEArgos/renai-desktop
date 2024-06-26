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

#include <Declarations.h>

__STRUCT(Window, {
    GLFWwindow* inner_window;
    i32 window_width;
    i32 window_height;
});

__CREATE_STRUCT_KILLFAIL(Window)
CreateWindow(i32 width, i32 height, const char* caller);

__INLINE __GET_STRUCT(GLFWwindow) GetInnerWindow(Window* window)
{
    return window->inner_window;
}

__INLINE u8 CheckWindowValidity(Window* window)
{
    return window->inner_window != NULL;
}

__INLINE u8 GetWindowShouldClose(Window* window)
{
    return glfwWindowShouldClose(GetInnerWindow(window));
}

__INLINE void KillWindow(Window* window)
{
    glfwDestroyWindow(window->inner_window);
    free(window);
    // PrintWarning("Killed window '%s'.", TITLE);
    glfwTerminate();
}

typedef enum WindowFullscreenState
{
    windowed,
    maximized,
    borderless
} WindowFullscreenState;

void SetWindowFullscreenState(Window* win, WindowFullscreenState state);

#endif // _RENAI_WINDOW_
