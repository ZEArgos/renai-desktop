#include "Application.h" // Mother header of the file, provides all functions and structures.
#include "Libraries.h" // Include the various functions to do with initializing libraries.
#include "Logger.h" // Include the logic needed to output information, either to the standard output or error output.

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
    PrintSuccess("Allocated memory for the main application structure.");

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
    if (GetInnerWindow(application->window) == NULL) exit(-1);

    // Create the renderer for the application, using the window's width and
    // height to cook up the projection matrix. If this fails, kill the
    // application.
    application->renderer = CreateRenderer(default_width, default_height);
    if (!CheckRendererValidity(application->renderer)) exit(-1);

    // Create the keybuffer, where we'll store the keys that have been
    // pressed in the last 50 cycles and are awaiting their time to be
    // pressed again.
    application->keybuffer = CreateKeyBuffer();

    // Return the allocated memory.
    return application;
}

__KILLFAIL RunApplication(Application* application)
{
    while (!GetWindowShouldClose(application->window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        glEnable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        HandleInput(application->keybuffer, application->window);
        RenderWindowContent(application->renderer);

        glfwPollEvents();
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
