#include "Application.h"
#include "Libraries.h"
#include "Logger.h"

// The application defined within the entry file. This is here since we need its
// members for the key callback.
extern Application* renai;

/**
 * @brief The key callback of the application; this fires every single time a
 * key is pressed/actioned upon.
 * @param window The window that was focused when the key was pressed.
 * @param key The key pressed.
 * @param scancode The scancode of the key (unused).
 * @param action The action taken upon the key.
 * @param mods Modifiers to the key action.
 */
void _key_callback(GLFWwindow* window, int key, int scancode, int action,
                   int mods)
{
    HandleInput(renai->updater, renai->window, renai->delta_time, key, action);
}

__CREATE_STRUCT_KILLFAIL(Application) CreateApplication(const char* caller)
{
// Get the current date and time, and then log that value to the console if
// we're in debug mode.
#ifdef DEBUG_MODE
    char current_date[64];
    GetDateString(current_date, 64);
    PrintWarning("Beginning a new session on %s.", current_date);
#endif

    Application* application = malloc(sizeof(struct Application));
    if (application == NULL)
        PrintError(
            "Failed to properly allocate space for the application. Code: %d.",
            errno);
    PrintSuccess(
        "Allocated memory for the main application structure: %d bytes.",
        sizeof(Application));
    // Since we assume that we're loading into a startup menu, set the startup
    // state to "menu".
    application->current_application_state = false;

    InitializeGLFW();
    const GLFWvidmode* resolution = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (resolution == NULL)
        PrintError("Failed to get the user's primary monitor's resolution. "
                   "Please report this.");
    // Set the application's frame cap to the monitor's refresh rate.
    ChangeApplicationFrameCap(1);

    *(i32*)&application->screen_width = resolution->width;
    *(i32*)&application->screen_height = resolution->height;

    i32 default_width = resolution->width / 1.25,
        default_height = resolution->height / 1.25;

    PrintSuccess(
        "Calculated application dimensions. Screen: %dx%d, Window: %dx%d.",
        application->screen_width, application->screen_height, default_width,
        default_height);

    application->window = CreateWindow(default_width, default_height, __func__);
    // Set the window's key press callback, so we don't have to call a function
    // redundantly every render call, and can instead rely on GLFW to tell us
    // when keys are pressed.
    glfwSetKeyCallback(GetInnerWindow(application->window), _key_callback);

    application->renderer =
        CreateRenderer(default_width, default_height, __func__);

    // Create the keybuffer, where we'll store the keys that have been
    // pressed in the last 25 cycles and are awaiting their time to be
    // pressed again.
    application->updater = CreateUpdater(50);

    return application;
}

__KILLFAIL RunApplication(Application* application)
{
    i64 last_frame_time = GetCurrentTime();
    f32 current_fps = 120.0f;
    u8 frames_past = 0;

    while (!GetWindowShouldClose(application->window))
    {
        frames_past++;

        i64 current_frame_time = GetCurrentTime();
        application->delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        // Clear the background of the window to black and then clear the
        // window's buffers.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_SCISSOR_TEST);
        // Clear the color buffer from the sides of the scissor box, cutting
        // down our rendering area to a 1:1 box in the center.
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        RenderWindowContent(application->renderer);
        UpdateWindowContent(application->updater, application->delta_time);

        glfwSwapBuffers(application->window->inner_window);

        if (application->current_application_state)
        {
            // Recalculate the possible framerate of the application. The reason
            // this is "possible" and not just the framerate is because we only
            // draw in time with the monitor's refresh rate. This is simply for
            // diagnostics, telling the user how well the application is
            // running.
            current_fps = CalculatePossibleFramerate(current_frame_time);

            if (frames_past == 5)
            {
                char window_title[64];
                // Format the string accordingly, but make sure we don't
                // overflow the buffer.
                snprintf(window_title, 64, "%s -- %.2f FPS", TITLE,
                         current_fps);
                glfwSetWindowTitle(GetInnerWindow(application->window),
                                   window_title);
            }

            // Poll for events like key pressing, resizing, and the like.
            glfwPollEvents();
        }
        else
        {
            // Make certain that the window title doesn't currently have an FPS
            // counter on it, and if it does, reset it to the original state.
            if (strcmp(glfwGetWindowTitle(GetInnerWindow(application->window)),
                       TITLE) != 0)
                glfwSetWindowTitle(GetInnerWindow(application->window), TITLE);

            // Poll for events like key pressing, resizing, and the like, but
            // impose a delay until an event triggers. We use this for menus
            // since we don't need to process things while the user isn't doing
            // anything.
            glfwWaitEvents();
        }
    }
    PrintSuccess("Got through the application's main loop successfully.");
}

__KILL DestroyApplication(Application* application, const char* caller)
{
    if (application == NULL)
    {
        PrintError("Tried to destroy the application '%s' before it was "
                   "created. Please report this bug.",
                   NAME);
    }

    KillWindow(application->window);
    KillRenderer(application->renderer);
    KillUpdater(application->updater);
    PrintWarning("Killed the application '%s's resources.", NAME);

    // Free the memory shell associated with the application structure and print
    // what we did.
    free(application);
    PrintWarning("Freed the memory of application '%s'.\n\n", NAME);

    exit(0);
}

void SwapApplicationType(Application* application)
{
    // Since we're working with a boolean value, just negate the previous value.
    application->current_application_state =
        !application->current_application_state;
}

void ChangeApplicationFrameCap(u8 new_cap) { glfwSwapInterval(new_cap); }
