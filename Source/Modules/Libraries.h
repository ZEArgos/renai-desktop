/**
 * @file Libraries.h
 * @author Zenais Argos
 * @brief Provides functions to handle and initialize the various libraries
 * Renai uses.
 * @date 2024-05-25
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_LIBRARIES_
#define _RENAI_LIBRARIES_

// I need the silly typedefs provided by this file.
#include "Declarations.h"

/**
 * @brief Print the last error GLFW ran into. Note that if, for some reason, the
 * PrintError method called within this function fails, the application will
 * exit. This is because I figure that if a GLFW error happens, AND my message
 * functions don't work either, something is seriously fucked.
 */
null PrintGLFWError(void);

/**
 * @brief Initializes the GLFW library, providing window management
 * functionality. Kills the application on failure.
 */
null InitializeGLFW(void);

/**
 * @brief A message-logging wrapper around the @ref glfwTerminate function.
 */
null KillGLFW(void);

/**
 * @brief Initializes the GLAD library and fetches OpenGL for use in the
 * application. Terminates the application on failure.
 */
null InitializeGLAD(void);

#endif // _RENAI_LIBRARIES_
