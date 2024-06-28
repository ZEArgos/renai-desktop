/**
 * @file Libraries.h
 * @author Zenais Argos
 * @brief Provides functions to handle and initialize the various
 * libraries Renai uses.
 * @date 2024-05-25
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_LIBRARIES_
#define _RENAI_LIBRARIES_

// Provides the type definitions and utility functions needed to run
// these functions.
#include <Declarations.h>

/**
 * @brief Initializes the GLFW library, providing window management
 * functionality. This kills the application on failure.
 */
__KILLFAIL InitializeGLFW(void);

/**
 * @brief Initializes the GLAD library and fetches OpenGL for use in
 * the application. Terminates the application on failure.
 */
__KILLFAIL InitializeGLAD(void);

/**
 * @brief A wrapper around the two functions @ref glViewport and @ref
 * glScissor, since both take the same parameters in this application.
 * @param lower_left_x The lower left corner's X coordinate.
 * @param lower_left_y The lower left corner's Y coordinate.
 * @param width The width of the box.
 * @param height The height of the box.
 */
__INLINE void SetOpenGLViewport(i32 lower_left_x, i32 lower_left_y,
                                i32 width, i32 height)
{
    glViewport(lower_left_x, lower_left_y, width, height);
    glScissor(lower_left_x, lower_left_y, width, height);
}

#endif // _RENAI_LIBRARIES_
