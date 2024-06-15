#include "Application.h" // Mother header of the file, provides all functions and structures.
#include "Libraries.h" // Include the various functions to do with initializing libraries.
#include "Logger.h" // Include the logic needed to output information, either to the standard output or error output.

// The application defined within the entry file. This is here since we need its
// members for the key callback.
extern Application* renai;

// A count of the number of render cycles since the last key press. This is only
// available within this file.
static u64 cycles_since_last_key = 1;

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
    // Send the key to our handling function.
    HandleInput(renai->keybuffer, renai->window, key, action,
                cycles_since_last_key);
    // Reset the cycles since the last key press.
    cycles_since_last_key = 1;
}

__CREATE_STRUCT_KILLFAIL(Application) CreateApplication(const char* caller)
{
    // Allocate enough space for 64 characters, more than enough room to store
    // the date string.
    char current_date[64];
    // Get the current date and time, and then log that value to the console if
    // we're in debug mode.
    GetDateString(current_date);
    PrintWarning("Beginning a new session on %s.", current_date);

    // Allocate enough room for the application's contents. If this fails, kill
    // the application.
    Application* application = malloc(sizeof(struct Application));
    if (application == NULL)
        PrintError(
            "Failed to properly allocate space for the application. Code: %d.",
            errno);
    PrintSuccess(
        "Allocated memory for the main application structure: %d bytes.",
        sizeof(Application));

    // Initialize GLFW. This method kills the process on failure, so we
    // don't check for any errors.
    InitializeGLFW();

    // Try to get the user's primary monitor's video mode (resolution
    // information). If this fails, kill the application after printing an
    // error.
    const GLFWvidmode* resolution = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (resolution == NULL)
        PrintError("Failed to get the user's primary monitor's resolution. "
                   "Please report this.");

    // Set the screen width and height of the application to the height and
    // width of the primary monitor.
    *(i32*)&application->screen_width = resolution->width;
    *(i32*)&application->screen_height = resolution->height;
    // Set the default width and height of the window and renderer. This is
    // simply a certain portion of the screen as a whole.
    i32 default_width = resolution->width / 1.25,
        default_height = resolution->height / 1.25;
    // Print some dimension-related information for the purpose of screen size
    // referencing during debug.
    PrintSuccess(
        "Calculated application dimensions. Screen: %dx%d, Window: %dx%d.",
        application->screen_width, application->screen_height, default_width,
        default_height);

    // Create the application window, exiting the process if/when a process
    // occurs.
    application->window = CreateWindow(default_width, default_height, __func__);
    if (!CheckWindowValidity(application->window)) exit(-1);
    // Set the window's key press callback, so we don't have to call a function
    // redundantly every render call, and can instead rely on GLFW to tell us
    // when keys are pressed.
    glfwSetKeyCallback(GetInnerWindow(application->window), _key_callback);

    // Create the renderer for the application, using the window's width and
    // height to cook up the projection matrix. If this fails, kill the
    // application.
    application->renderer =
        CreateRenderer(default_width, default_height, __func__);
    if (!CheckRendererValidity(application->renderer, __func__)) exit(-1);

    // Create the keybuffer, where we'll store the keys that have been
    // pressed in the last 25 cycles and are awaiting their time to be
    // pressed again.
    application->keybuffer = CreateKeyBuffer();

    // Return the allocated memory.
    return application;
}

__KILLFAIL RunApplication(Application* application)
{
    // While the application's window shouldn't be closed, run through the
    // render loop.
    while (!GetWindowShouldClose(application->window))
    {
        // Clear the background of the window to black.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Clear the color and depth buffers.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Now, clear the background of the rendering area to red.
        //! This is temporary until I build the SceneRenderer.
        // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        // Enable the OpenGL scissor mechanics.
        glEnable(GL_SCISSOR_TEST);
        // Clear the color buffer from the sides of the scissor box, cutting
        // down our rendering area to a 1:1 box in the center.
        glClear(GL_COLOR_BUFFER_BIT);
        // Disable the scissor mechanics.
        glDisable(GL_SCISSOR_TEST);

        // Render the contents of the window.
        RenderWindowContent(application->renderer);
        // Increment the number of render cycles it's been since the last time a
        // key was pressed.
        cycles_since_last_key++;

        // Poll for events like key pressing, resizing, and the like.
        glfwPollEvents();
        // Swap the front and back buffers of the application.
        glfwSwapBuffers(application->window->inner_window);
    }
}

__KILL DestroyApplication(Application* application, const char* caller)
{
    // If the passed application is not created, print an error to the error
    // output and exit the process with an error code.
    if (application == NULL)
    {
        PrintError("Tried to destroy the application '%s' before it was "
                   "created. Please report this bug.",
                   NAME);
    }

    // Kill the application's resources, freeing all memory associated with
    // them as well.
    KillWindow(application->window);
    KillRenderer(application->renderer);
    KillKeyBuffer(application->keybuffer);
    KillGLFW();
    // Print the task we've just completed.
    // PrintWarning("Killed the application '%s's resources.", NAME);

    // Free the memory shell associated with the application structure.
    free(application);
    // Exit the process with a success code.
    exit(0);
}
