/**
 * @file Application.h
 * @author Zenais Argos
 * @brief Provides the methods needed to run Renai. This should be the
 * only include from the application's entrypoint.
 * @date 2024-05-26
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_APPLICATION_
#define _RENAI_APPLICATION_

// Provides the functionality and data structures needed to render
// content onto a given window.
#include <Renderer.h>
// Provides the functionality needed to poll for keypresses and handle
// them accordingly.
#include <Updater.h>
// Provides the data structures and functionality needed to handle
// window management.
#include <Window.h>

/**
 * @brief A structuring for all the data needed by an application.
 * This is 32 bytes large.
 */
typedef struct Application
{
    /**
     * @brief This is a boolean flag representing the current state of
     * the application; true for gameplay, false for menu. This
     * decides how often the game calls the render function.
     */
    bool current_application_state;
    /**
     * @brief The time difference it took to render the current frame
     * and the last. This value to used to normalize updating.
     */
    f32 delta_time;
    /**
     * @brief The width of the user's primary monitor, for use in many
     * calculations and borderless fullscreening the window.
     */
    const i32 screen_width;
    /**
     * @brief The height of the user's primary monitor, for use in
     * many calculations and borderless fullscreen toggling of the
     * window.
     */
    const i32 screen_height;
    /**
     * @brief The application's window, onto which the game will be
     * rendered.
     */
    Window* window;
    /**
     * @brief The application's renderer, to take care of the whole
     * "rendering onto the window" thing.
     */
    Renderer* renderer;
    /**
     * @brief This is the updater of the application; the structure
     * that handles processing, keystrokes, and the like.
     */
    Updater* updater;
} Application;

/**
 * @brief Create an application and initialize its starting processes.
 * This should really only be called once. Kills the process on
 * failure.
 * @param caller The caller of the function, should be "main".
 */
__CREATE_STRUCT_KILLFAIL(Application)
CreateApplication(const char* caller);

/**
 * @brief Run an application's main loop methods until the key window
 * is closed. This includes rendering, keyboard polling, and more.
 * @param application The application to run.
 */
__KILLFAIL RunApplication(Application* application);

/**
 * @brief Destroy an application and all of its innards. This function
 * @b always exits the process on completion, regardless of
 * anything--positive or negative--that may have occurred.
 * @param application The application to destroy.
 * @param caller The caller of the function, should be "main".
 */
__KILL DestroyApplication(Application* application,
                          const char* caller);

/**
 * @brief Swap the application's current state; from menu to gameplay
 * or gameplay to menu.
 * @param application The application whose state to swap to swap.
 */
void SwapApplicationType(Application* application);

/**
 * @brief Set the application frame cap, with the formula of monitor
 * hertz /
 * @param new_cap.
 * @param new_cap The new value to divide the monitor refresh rate
 * with for updating. A value of 0 turns off VSYNC / the frame cap
 * entirely.
 */
void ChangeApplicationFrameCap(u8 new_cap);

#endif // _RENAI_APPLICATION_
