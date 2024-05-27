#include "Application.h"
#include "Libraries.h"
#include "Logger.h"
#include "Window.h"

/**
 * @brief The application data structure of the game. Contains the key window
 * and some global data flags.
 */
struct
{
    u8 initialized;
    Window* key_window;
} _application = {0, NULL};

GLFWwindow* GetKeyWindow(void)
{
    // Make sure the application is initialized, and if not warn about the
    // access attempt.
    if (_application.initialized == 0 || _application.key_window == NULL)
    {
        PrintWarning("Tried to access the application's key window before its "
                     "initialization.");
        return NULL;
    }
    // Return the key window's inner window. Note that we don't check to make
    // sure THIS window is initialized, so it's up to the function caller.
    return _application.key_window->inner_window;
}

null InitializeApplication(void)
{
    if (_application.initialized == 1)
        return;

    InitializeGLFW();

    // Allocate some space for the title string.
    char title_string[WINDOW_MAX_TITLE_LENGTH];
    // Try to concatenate the version into the string. If this fails, print
    // the error and return nothing.
    if (snprintf(title_string, WINDOW_MAX_TITLE_LENGTH, "%s | %s", TITLE,
                 VERSION) < 0)
    {
        PrintError("Failed to create title string for window. Code: %d.",
                   errno);
        exit(-1);
    }

    _application.key_window = CreateKeyWindow(150, 150, 10, 50, title_string);
    if (_application.key_window == NULL)
        exit(-1);

    _application.initialized = 1;
}

u8 RunApplication(void)
{
    if (_application.initialized == 0)
    {
        PrintWarning("Tried to run the application before initialization.");
        return FAILURE;
    }

    while (!glfwWindowShouldClose(GetKeyWindow()))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(WINDOW_BACKGROUND_R, WINDOW_BACKGROUND_G,
                     WINDOW_BACKGROUND_B, 1.0f);

        glfwPollEvents();
        glfwSwapBuffers(GetKeyWindow());
    }

    return SUCCESS;
}

null DestroyApplication(void)
{
    KillWindow(_application.key_window);
    KillGLFW();

    _application.initialized = 0;
}
