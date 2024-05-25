#include <Logger.h>
#include <Window.h>

i32 main(void)
{
    InitializeGLFW();
    GLFWwindow* win = CreateWindow(150, 150, 10, 50, "Testing");
    InitializeGLAD();

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
