#include "Declarations.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>

u16 terminal_width = 0;
i64 start_time = 0;

#if defined(linux)
null GetTerminalWidth(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        printf("%d", errno);
        exit(-1);
    }
    terminal_width = w.ws_col;
}

i64 GetCurrentTime(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);

    if (start_time == 0)
    {
        start_time = (int64_t)(time.tv_sec) * 1000 + time.tv_usec / 1000;
        return 0;
    }
    else
        return start_time -
               ((int64_t)(time.tv_sec) * 1000 + time.tv_usec / 1000);
}
#elif defined(WIN32)
// windows terminal grabber code goes here
#endif
