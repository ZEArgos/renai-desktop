#include "Application.h" // Mother header of the file, provides all functions and structures.
#include "Libraries.h" // Include the various functions to do with initializing libraries.
#include "Logger.h" // Include the logic needed to output information, either to the standard output or error output.

__CREATE_STRUCTURE_KILLFAIL(Application) CreateApplication(void)
{
    // Allocate enough space for 64 characters, more than enough room to store
    // the date string.
    char current_date[64];
    // Get the current date and time, and then log that value to the console if
    // we're in debug mode.
    GetDateString(current_date);
    PrintWarning("Beginning a new session on %s.", current_date);

    // Allocate enough room for the application's contents.
    Application* application = malloc(sizeof(struct Application));

    // Initialize GLFW. This method kills the process on failure, so we don't
    // check for any errors.
    InitializeGLFW();

    // Try to get the user's primary monitor's video mode (resolution
    // information). If this fails, kill the application after printing an
    // error.
    const GLFWvidmode* resolution = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (resolution == NULL)
    {
        PrintError("Failed to get the user's primary monitor's resolution. "
                   "Please report this.");
    }

    // Set the screen width and height of the application to the height and
    // width of the primary monitor.
    *(i32*)&application->screen_width = resolution->width;
    *(i32*)&application->screen_height = resolution->height;

    //
    application->window =
        CreateKeyWindow(resolution->width / 1.25, resolution->height / 1.25);
    if (application->window->inner_window == NULL) exit(-1);

    application->renderer = CreateRenderer(
        resolution->width / 1.25, resolution->height / 1.25, RENDERER_MAIN_UID);
    if (application->renderer->shader_list == NULL) exit(-1);

    application->keybuffer = CreateKeyBuffer();

    return application;
}

__KILLFAIL RunApplication(Application* application)
{
    while (!glfwWindowShouldClose(GetInnerWindow(application->window)))
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

__KILL DestroyApplication(Application* application)
{
    // If the passed application is not created, print an error to the error
    // output and exit the process with an error code.
    if (application == NULL)
    {
        PrintError("Tried to destroy the application '%s' before it was "
                   "created. Please report this bug.",
                   NAME);
    }

    // Kill the application's resources, freeing all memory associated with them
    // as well.
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
