#include "Application.h" // Mother header of the file, provides all functions and structures.
#include "Libraries.h" // Provides functions to initialize the various used libraries.
#include "Logger.h" // Provides the debug logger functionality.
#include "Renderer.h" // Include the various functions and data structures needed to render objects.
#include "Window.h" // Includes the various structures/functions needed to create and manage windows.

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

    // Get the primary monitor's video information and store it. This is used in
    // many calculations throughout the process.
    const GLFWvidmode* resolution = glfwGetVideoMode(glfwGetPrimaryMonitor());
    _application.screen_width = resolution->width;
    _application.screen_height = resolution->height;

    // Try to initialize the application's key window. If this fails, fail the
    // application's process.
    _application.window = CreateKeyWindow();
    if (_application.window.inner_window == NULL) exit(-1);

    // Try to initialize the renderer. If this fails, the application will
    // self-destruct.
    _application.renderer =
        CreateRenderer(resolution->width / 1.25, resolution->height / 1.25);
    if (_application.renderer.shader_list_head == NULL) exit(-1);

    // Set the initialization flag of the application to true, so this
    // function is not called twice.
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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        // Clip the edges of the scissor box, using our beautiful built-in
        // OpenGL tooling that totally doesn't look ridiculous.
        glEnable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        // Render the window's actual content, beyond its background.
        RenderWindowContent(&_application.renderer);

        // Render the contents of the window. This kills the application on
        // failure, so don't worry about error checking.

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
    DestroyRenderer(&_application.renderer);
    KillGLFW();

    // Set the initialization bits back to the original values.
    _application.initialized = 0;
}
