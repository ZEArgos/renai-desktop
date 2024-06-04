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

/**
 * @brief Initializes the GLFW library, providing window management
 * functionality. This kills the application on failure.
 */
void InitializeGLFW(void);

/**
 * @brief Initializes the GLAD library and fetches OpenGL for use in the
 * application. Terminates the application on failure.
 */
void InitializeGLAD(void);

/**
 * @brief A message-logging wrapper around the @ref glfwTerminate function.
 */
void KillGLFW(void);

#endif // _RENAI_LIBRARIES_
