#include "Application.h"
#include "Libraries.h"
#include "Logger.h"
#include "Renderer.h"
#include "Window.h"

/**
 * @brief The application data structure of the game. Contains the key window
 * and some global data flags.
 */
struct Application
{
    u8 initialized;
    Window key_window;
    Renderer renderer;
} _application = {0};

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

    // Return the key window's inner window, using a secondary check system
    // implemented in the Window.h file.
    return GetInnerWindow(&_application.key_window);
}

null InitializeApplication(void)
{
    // Check to make sure the application hasn't already been initialized, and
    // if it has, do not proceed.
    if (_application.initialized == 1)
    {
        PrintWarning("Tried to initialize the application twice.");
        return;
    }

    // Log the beginning of the session, for time-keeping purposes.
    char time_string[128];
    GetDateString(time_string);
    PrintWarning("Beginning a new session on %s.", time_string);

    // Initialize GLFW. This kills the application completely on failure, so we
    // don't bother checking for errors.
    InitializeGLFW();

    // Try to initialize the application's key window. if this fails, fail the
    // application.
    _application.key_window = CreateKeyWindow(10, 50, TITLE);
    if (GetInnerWindow(&_application.key_window) == NULL)
        exit(-1);

    // Try to initialize the renderer. If this fails, kill the application.
    const GLFWvidmode* resolution = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (InitializeRenderer(resolution->width, resolution->height) == FAILURE)
        exit(-1);

    // Set the initialization flag of the application.
    _application.initialized = 1;
}

u8 RunApplication(void)
{
    // If the application hasn't yet been initialized, don't allow this function
    // to continue any further, and log the issue to the console.
    if (_application.initialized == 0)
    {
        PrintError("Tried to run the application before initialization.");
        return FAILURE;
    }
    PrintSuccess("Beginning the application's main loop.");

    glEnable(GL_DEPTH_TEST);
    const GLFWvidmode* resolution = glfwGetVideoMode(glfwGetPrimaryMonitor());
    // While the window shouldn't be closed, run the render / update loop.
    while (!glfwWindowShouldClose(GetKeyWindow()))
    {
        // Clear the color buffer and paint it with a clear white.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Render the contents of the window. If this fails, kill the function.
        if (!_application.renderer.RenderWindowContent(
                &_application.renderer, resolution->width, resolution->height))
        {
            PrintError("Failed to properly render the window's contents.");
            return FAILURE;
        }

        // Poll for any events like keyboard pressing or resizing.
        glfwPollEvents();
        // Swap the front and back framebuffers.
        glfwSwapBuffers(GetKeyWindow());
    }
    PrintSuccess("Main loop finished.");

    // Return that the function was successful.
    return SUCCESS;
}

null DestroyApplication(void)
{
    // Make sure we clean up after ourselves.
    KillWindow(&_application.key_window);
    KillGLFW();
    DestroyRenderer();

    // Set the initialization bits back to the original values.
    _application.initialized = 0;
}
