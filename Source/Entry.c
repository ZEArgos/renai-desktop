#include "Application.h" // Provides literally the entire engine.

#include <Map.h>

/**
 * @brief Start the application. This function does very little but begin the
 * application and make certain all systems are running.
 * @return A 32-bit integer flag, typically only <0 for failure and 0 for
 * success.
 */
i32 main(void)
{
    // Initialize the application and try to run its loop. If the loop fails,
    // the process will self-destruct, so don't worry about error checking.
    InitializeApplication();
    RunApplication();

    Map* created_map = CreateMap(signed32, unsigned32, 10);

    AppendMapItem(created_map, 'c', 23);
    printf("%d\n", GetMapItemValueUnsigned32(created_map, 'c'));

    DestroyMap(created_map);

    // Destroy/free all allocated memory and get ready to exit the
    // application entirely.
    DestroyApplication();
    return 0;
}
