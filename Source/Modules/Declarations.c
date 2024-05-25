#include "Declarations.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>

u16 terminal_width = 0;
i64 start_time = 0;
char last_time_string[256] = "0 seconds, 0 minutes, 0 hours";

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
        return ((int64_t)(time.tv_sec) * 1000 + time.tv_usec / 1000) -
               start_time;
}
null GetTimeString(void)
{
    i64 seconds = GetCurrentTime(), minutes = 0, hours = 0;
    if (seconds / 60 > 0)
    {
        minutes = seconds / 60;
        seconds = seconds % 60;
    }
    if (minutes / 60 > 0)
    {
        hours = minutes / 60;
        minutes = minutes % 60;
    }

    sprintf(last_time_string, "%ld seconds, %ld minutes, %ld hours", seconds,
            minutes, hours);
}
#elif defined(_WIN32)
// windows terminal grabber code goes here
#endif
