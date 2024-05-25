#include "Logger.h"

#ifdef DEBUG_MODE
#include <stdarg.h>
#include <string.h>

const char *status_strings[2] = {"  SUCCESS  ", "   ERROR   "},
           *status_colors[2] = {"\033[32m", "\033[31m"};
u8 PrintMessage(u8 state, string message, ...)
{
    if (terminal_width == 0)
        GetTerminalWidth();
    if (start_time == 0)
        (null) GetCurrentTime();

    va_list args;
    va_start(args, message);

    printf("%s[%s]\033[0m\033[33m  MESSAGE:\033[0m ", status_colors[state],
           status_strings[state]);
    vprintf(message, args);

    char* time_string = _GetTimeString();
    printf(
        "%.*s", (i32)(terminal_width - strlen(time_string)),
        "                                                                   "
        "                                                                   ");
    printf("\033[33mTIME:\033[0m %s\n", time_string);
    free(time_string);

    return SUCCESS;
}
#else
u8 PrintMessage(u8 state, string message, ...) { return SUCCESS; }
#endif
