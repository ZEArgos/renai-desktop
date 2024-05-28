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
    return ((int64_t)(time.tv_sec) * 1000 + time.tv_usec / 1000) - start_time;
}
#elif defined(_WIN32)
// windows terminal grabber code goes here
#endif

null GetTimeString(void)
{
    i64 ms = GetCurrentTime(), s = 0, m = 0;
    if (ms > 1000)
    {
        s = ms / 1000;
        ms = ms % 1000;
    }
    if (s > 60)
    {
        m = s / 60;
        s = s % 60;
    }

    FormatTimeString(ms, s, m);
}

#include <limits.h>
i32 _DigitCount(i64 n)
{
    if (n < 0)
        return _DigitCount((n == INT_MIN) ? INT_MAX : -n);
    if (n < 10)
        return 1;
    return 1 + _DigitCount(n / 10);
}

null FormatTimeString(i64 ms, i64 s, i64 m)
{
    i32 ms_digit_count = 4 - _DigitCount(ms),
        s_digit_count = 2 - _DigitCount(s), m_digit_count = 5 - _DigitCount(m);

    char last_string_buffer[MAX_TIME_STRING_LENGTH];
    snprintf(
        last_time_string, ms_digit_count,
        "000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000");
    sprintf(last_string_buffer, "%ldms, ", ms);
    strncat(
        last_string_buffer,
        "000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000",
        s_digit_count);
    strcat(last_time_string, last_string_buffer);
    sprintf(last_string_buffer, "%lds, ", s);
    strncat(
        last_string_buffer,
        "000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000",
        m_digit_count);
    strcat(last_time_string, last_string_buffer);
    sprintf(last_string_buffer, "%ldm", m);
    strcat(last_time_string, last_string_buffer);
}
