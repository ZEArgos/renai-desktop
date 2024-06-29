#include <Application.h> // Provides literally the entire engine.

/**
 * @brief A variable in which is stored every bit of consistent data
 * the application needs during its runtime. Initialized within @ref
 * main.
 */
Application* renai;

/**
 * @brief Start the application. This function does very little but
 * begin the application and make certain all systems are running.
 * @return A 32-bit integer flag, typically only <0 for failure and 0
 * for success.
 */
i32 main(void)
{
    char* scene_names[1] = {"test"};
    char* scene_descriptions[1] = {"a quick test scene"};
    char* texture_paths[1] = {"texture_missing.jpg"};
    CreateScenes(scene_names, scene_descriptions, texture_paths, 1,
                 1);
    // exit(0);

    // Initialize the application and try to run its loop. If the loop
    // fails, the process will self-destruct, so don't worry about
    // error checking.
    renai = CreateApplication();
    RunApplication(renai);

    // Destroy/free all allocated memory and get ready to exit the
    // application entirely.
    KillApplication(renai);
}
