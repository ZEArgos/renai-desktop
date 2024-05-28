#include "Application.h"
#include "Libraries.h"
#include "Logger.h"
#include "Shader.h"
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
    if (_application.initialized == 0)
    {
        PrintWarning("Tried to access the application's key window before its "
                     "initialization.");
        return NULL;
    }
    // Return the key window's inner window, using a secondary check system
    // implemented in the Window.h file.
    return GetInnerWindow(_application.key_window);
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

    // Initialize GLFW. This kills the application completely on failure, so we
    // don't bother checking for errors.
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

    // Try to initialize the application's key window. if this fails, fail the
    // application.
    _application.key_window = CreateKeyWindow(10, 50, title_string);
    if (_application.key_window == NULL)
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

    u32 shader = LoadShader("basic");

    float vertices[] = {
        // positions         // colors
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // While the window shouldn't be closed, run the render / update loop.
    while (!glfwWindowShouldClose(GetKeyWindow()))
    {
        // Clear the color buffer and paint it with a clear white.
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(APPLICATION_BACKGROUND_R, APPLICATION_BACKGROUND_G,
                     APPLICATION_BACKGROUND_B, 1.0f);

        if (!UseShader(shader))
            return FAILURE;
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Poll for any events like keyboard pressing or resizing.
        glfwPollEvents();
        // Swap the front and back framebuffers.
        glfwSwapBuffers(GetKeyWindow());
    }
    PrintSuccess("Main loop finished.");

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Return that the function was successful.
    return SUCCESS;
}

null DestroyApplication(void)
{
    // Make sure we clean up after ourselves.
    KillWindow(_application.key_window);
    KillGLFW();

    // Set the initialization bits back to the original values.
    _application.initialized = 0;
}
