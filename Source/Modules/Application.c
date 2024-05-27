#include "Application.h"
#include "Libraries.h"
#include "Logger.h"
#include "Window.h"

struct
{
    u8 initialized;
    GLFWwindow* key_window;
    f32 background_r, background_g, background_b;
} _application = {0, NULL, .0f, .0f, .0f};

null InitializeApplication(string title, f32 bgr, f32 bgg, f32 bgb)
{
    if (_application.initialized == 1)
        return;

    InitializeGLFW();

    _application.background_r = bgr;
    _application.background_g = bgg;
    _application.background_b = bgb;

    // Allocate some space for the title string.
    char title_string[WINDOW_MAX_TITLE_LENGTH];
    // Try to concatenate the version into the string. If this fails, print
    // the error and return nothing.
    if (snprintf(title_string, WINDOW_MAX_TITLE_LENGTH, "%s | %s", title,
                 VERSION) < 0)
    {
        PrintError("Failed to create title string for window, expected; '%s'. "
                   "Code: %d.",
                   title, errno);
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

    while (!glfwWindowShouldClose(_application.key_window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(_application.background_r, _application.background_g,
                     _application.background_b, 1.0f);

        glfwPollEvents();
        glfwSwapBuffers(_application.key_window);
    }

    return SUCCESS;
}

null DestroyApplication(void)
{
    KillWindow(_application.key_window);
    KillGLFW();

    _application.initialized = 0;
}
