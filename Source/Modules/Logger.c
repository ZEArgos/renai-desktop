#include "Logger.h"
#include <stdarg.h>

/**
 * @brief The reported width of the terminal in columns.
 */
u64 terminal_width = 0;

#if defined(linux)

#include <sys/ioctl.h>
#include <unistd.h>
null _GetTerminalWidth(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    terminal_width = w.ws_col;
}

#else
null _GetTerminalWidth(void) {}
#endif

u8 _Print(u8 state, string msg, va_list args)
{
    if (terminal_width == 0)
        _GetTerminalWidth();

    return SUCCESS;
}

u8 PrintMessage(string task_msg, ...)
{
    va_list args;
    va_start(args, task_msg);

    return _Print(0, task_msg, args);
}

u8 PrintError(string task_msg, ...)
{
    va_list args;
    va_start(args, task_msg);

    return _Print(1, task_msg, args);
}
