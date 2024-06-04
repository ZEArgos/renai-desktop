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
// Includes the various structures/functions needed to create and manage
// windows.
#include "Window.h"
// Include the various functions and data structures needed to render objects.
#include "Renderer.h"

/**
 * @brief The structuring of the application interface. This simply provides the
 * gears to run the engine, everything must be setup using functions and other
 * subdata.
 */
typedef struct Application
{
    /**
     * @brief A flag to tell the user if the application has yet been
     * initialized.
     */
    u8 initialized;
    /**
     * @brief The width of the primary monitor's framebuffer. This is used for a
     * couple calculations throughout the application, so it's probably prudent
     * to store its value.
     */
    f32 screen_width;
    /**
     * @brief The height of the monitor's framebuffer, kept for the same reasons
     * as the width.
     */
    f32 screen_height;
    /**
     * @brief The window of the application.-
     */
    Window window;
    /**
     * @brief The application's renderer.
     */
    Renderer renderer;
} Application;

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
 * DestroyApplication. This function will kill the application on failure.
 */
null RunApplication(void);

/**
 * @brief Kill the application. This should be the LAST function call from the
 * application's entrypoint. Nothing else needs to be called beyond this point,
 * as all data in my control has been freed.
 */
null DestroyApplication(void);

#endif // _RENAI_APPLICATION_
