/**
 * @file Application.h
 * @author Zenais Argos
 * @brief Provides the methods needed to run Renai. This should be the only
 * include from the application's entrypoint.
 * @date 2024-05-26
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_APPLICATION_
#define _RENAI_APPLICATION_

// Includes the various typedefs and utility functions needed to run the game.
#include "Declarations.h"

/**
 * @brief Get the inner_window object of the application's key window. This is
 * simply a getter. Returns NULL if the application hasn't been initialized.
 * @return A pointer to the GLFW interface window within the key window.
 */
GLFWwindow* GetKeyWindow(void);

/**
 * @brief Initialize the application class and start the game's functionality
 * loop. The game doesn't *run* until @ref RunApplication is called, but this
 * sets everything up to run, as if lining up runners before a track meet. This
 * function kills the application on failure, so no error checking against it is
 * needed.
 */
null InitializeApplication(void);

/**
 * @brief Run the application's main gameplay loop. This should be the last
 * function call from the application's entrypoint, unless it's @ref
 * DestroyApplication.
 * @return An 8-bit integer flag representing success or failure.
 */
u8 RunApplication(void);

/**
 * @brief Kill the application. This should be the LAST function call from the
 * application's entrypoint. Nothing else needs to be called beyond this point,
 * as all data in my control has been freed.
 */
null DestroyApplication(void);

#endif // _RENAI_APPLICATION_
