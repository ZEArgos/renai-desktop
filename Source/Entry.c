#include <Logger.h>
#include <Window.h>

i32 main(void)
{
    InitializeGLFW();
    GLFWwindow* win = CreateKeyWindow(150, 150, 10, 50, "testing");

    while (!glfwWindowShouldClose(win))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        glfwPollEvents();
        glfwSwapBuffers(win);
    }
    KillGLFW();
    return 0;
}
