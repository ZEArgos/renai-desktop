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

// Provides the functionality and data structures needed to render content onto
// a given window.
#include "Renderer.h"
// Provides the functionality needed to poll for keypresses and handle them
// accordingly.
#include "Updater.h"
// Provides the data structures and functionality needed to handle window
// management.
#include "Window.h"

/**
 * @brief A structuring for all the data needed by an application. This is 64
 * bytes large.
 */
__STRUCT(Application, {
    /**
     * @brief The width of the user's primary monitor, for use in many
     * calculations and borderless fullscreening the window.
     */
    const i32 screen_width;
    /**
     * @brief The height of the user's primary monitor, for use in many
     * calculations and borderless fullscreen toggling of the window.
     */
    const i32 screen_height;
    /**
     * @brief The application's window, onto which the game will be rendered.
     */
    Window* window;
    /**
     * @brief The application's renderer, to take care of the whole "rendering
     * onto the window" thing.
     */
    Renderer* renderer;
    /**
     * @brief The keybuffer of the application, which stores what keys were
     * pressed in the last 50 cycles and how long until they're free again.
     */
    KeyBuffer* keybuffer;
});

/**
 * @brief Create an application and initialize its starting processes. This
 * should really only be called once. Kills the process on failure.
 */
__CREATE_STRUCTURE_KILLFAIL(Application) CreateApplication(void);

/**
 * @brief Run an application's main loop methods until the key window is
 * closed. This includes rendering, keyboard polling, and more.
 * @param application The application to run.
 */
__KILLFAIL RunApplication(Application* application);

/**
 * @brief Destroy an application and all of its innards. This function @b always
 * exits the process on completion, regardless of anything--positive or
 * negative--that may have occurred.
 * @param application The application to destroy.
 */
__KILL DestroyApplication(Application* application);

#endif // _RENAI_APPLICATION_
