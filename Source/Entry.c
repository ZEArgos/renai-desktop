#include "Application.h"

i32 main(void)
{
    InitializeApplication(TITLE, 1.0f, 0.0f, 0.0f);
    RunApplication();

    DestroyApplication();
    return 0;
}
