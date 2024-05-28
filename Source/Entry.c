#include "Application.h"

i32 main(void)
{
    // Initialize the application and try to run its loop.
    InitializeApplication();
    if (RunApplication() == FAILURE)
        return -1;

    // Destroy/free all allocated memory and get ready for exiting.
    DestroyApplication();
    return 0;
}
