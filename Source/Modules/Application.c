#include "Application.h" // Mother header of the file, provides all functions and structures.
#include "Libraries.h" // Provides functions to initialize the various used libraries.
#include "Logger.h" // Provides the debug logger functionality.

/**
 * @brief The application data structure of the game. Contains the window
 * and some global data flags. This is private, since we don't exactly want it
 * being edited by random processes.
 */
Application _application = {0};

GLFWwindow* GetKeyWindow(void)
{
    // Make sure the application is initialized, and if not warn about the
    // access attempt.
    if (_application.initialized == 0)
    {
        PrintWarning("Tried to access the application's key window before its "
                     "initialization.");
        return NULL;
    }

    // Return the window's inner, GLFW-created window, using a secondary check
    // implemented in the Window.h file.
    return GetInnerWindow(&_application.window);
}

void InitializeApplication(void)
{
    // Check to make sure the application hasn't already been initialized, and
    // if it has, do not proceed.
    if (_application.initialized)
    {
        PrintWarning("Tried to initialize the application twice.");
        return;
    }

    // Log the beginning of the session, for time-keeping purposes.
    char time_string[64];
    GetDateString(time_string);
    PrintWarning("Beginning a new session on %s.", time_string);

    // Initialize GLFW. This kills the application completely on failure, so we
    // don't bother checking for errors.
    InitializeGLFW();

    // Try to initialize the application's key window. If this fails, fail the
    // application's process.
    _application.window = CreateKeyWindow();
    if (!CheckWindowValidity(&_application.window))
        exit(-1);

    // Get the primary monitor's video information and store it. This is used in
    // many calculations throughout the process.
    const GLFWvidmode* resolution = glfwGetVideoMode(glfwGetPrimaryMonitor());
    _application.screen_width = resolution->width;
    _application.screen_height = resolution->height;

    // Try to initialize the renderer. If this fails, kill the application.
    if (!InitializeRenderer())
        exit(-1);

    // Set the initialization flag of the application to true, so this function
    // is not called twice.
    _application.initialized = 1;
}

void RunApplication(void)
{
    // If the application hasn't yet been initialized, don't allow this function
    // to continue any further, and log the issue to the console.
    if (!_application.initialized)
    {
        PrintError("Tried to run the application before its initialization.");
        exit(-1);
    }
    PrintSuccess("Beginning the application's main loop.");

    // While the window shouldn't be closed, run the render / update loop.
    while (!glfwWindowShouldClose(GetKeyWindow()))
    {
        // Clear both the depth and color buffer, and then paint the window a
        // clear, opaque black.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Render the contents of the window. This kills the application on
        // failure, so don't worry about error checking.
        RenderWindowContent(&_application.renderer);

        // Poll for any events like keyboard pressing or resizing.
        glfwPollEvents();
        // Swap the front and back framebuffers.
        glfwSwapBuffers(GetKeyWindow());
    }
    PrintSuccess("Main loop finished.");
}

void DestroyApplication(void)
{
    // Make sure we clean up after ourselves, removing any window, renderer, and
    // GLFW data we may have accumulated.
    KillWindow(&_application.window);
    DestroyRenderer();
    KillGLFW();

    // Set the initialization bits back to the original values.
    _application.initialized = 0;
}
