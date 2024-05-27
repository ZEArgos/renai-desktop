/**
 * @file Application.h
 * @author Zenais Argos
 * @brief Provides the methods required for running Renai's basest functionality
 * easily.
 * @date 2024-05-26
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_APPLICATION_
#define _RENAI_APPLICATION_

// Gimme them typedefs baby
#include "Declarations.h"

/**
 * @brief Get the inner_window object of the application's key window. This is
 * simply a getter. Returns NULL if the application hasn't been initialized.
 * @return A pointer to the GLFW interface window within the key window.
 */
GLFWwindow* GetKeyWindow(void);

/**
 * @brief Initialize the application class and start the game's functionality
 * loop. The game doesn't run until @ref RunApplication is called, but this sets
 * everything up to run. The function kills the application on failure.
 */
null InitializeApplication(void);

/**
 * @brief Run the application's main loop. This should be the last function call
 * from the application's entrypoint, unless it's @ref DestroyApplication.
 * @return An integer flag representing success or failure.
 */
u8 RunApplication(void);

/**
 * @brief Kill the application. This should be the LAST function call from the
 * application's entrypoint.
 */
null DestroyApplication(void);

#endif // _RENAI_APPLICATION
