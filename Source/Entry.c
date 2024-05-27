#include "Application.h"

i32 main(void)
{
    InitializeApplication();
    if (RunApplication() == FAILURE)
        return -1;

    DestroyApplication();
    return 0;
}
