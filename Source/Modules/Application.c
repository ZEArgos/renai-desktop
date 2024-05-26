#include "Application.h"
#include "Libraries.h"
#include "Window.h"

struct
{
    GLFWwindow* key_window;
    f32 background_r, background_g, background_b;
} _application;

u8 InitializeApplication(string title, f32 bgr, f32 bgg, f32 bgb)
{
    InitializeGLFW();

    _application.background_r = bgr;
    _application.background_g = bgg;
    _application.background_b = bgb;

    _application.key_window =
        CreateKeyWindow(150, 150, 10, 50, "Renai | %s", VERSION);
    if (_application.key_window == NULL)
        return FAILURE;

    return SUCCESS;
}

u8 RunApplication(void)
{
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
}
