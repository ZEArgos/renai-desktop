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

u8 InitializeGLFW(void);
null KillGLFW(void);
u8 InitializeGLAD(void);
GLFWwindow* CreateWindow(string title, u16 width, u16 height, i32 x, i32 y);
null DestroyWindow(GLFWwindow* win);

#endif // _RENAI_WINDOW_
