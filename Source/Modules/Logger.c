#include "Logger.h"
#include "Declarations.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef DEBUG_MODE

// Provides variadic argument handling functionality.
#include <stdarg.h>

#include <limits.h>
i32 _DigitCount(i64 n)
{
    if (n < 0) return _DigitCount((n == INT_MIN) ? INT_MAX : -n);
    if (n < 10) return 1;
    return 1 + _DigitCount(n / 10);
}

#define MAX_TIME_STRING_LENGTH 256
char last_time_string[MAX_TIME_STRING_LENGTH];
void FormatTimeString(i64 ms, i64 s, i64 m)
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

u16 terminal_width = 0;
void GetTerminalWidth(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        printf("%d", errno);
        exit(-1);
    }
    terminal_width = w.ws_col;
}
i64 start_time = 0;
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
char last_time_string[256] = "0 seconds, 0 minutes, 0 hours";

void GetTimeString(void)
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

/**
 * @brief A constant array of char* to specify the messages given to the
 * terminal when a specific status is called upon.
 */
const char* status_strings[3] = {"  SUCCESS  ", "   ERROR   ", "  WARNING  "};
/**
 * @brief A constant array of color escape codes to indicate the status of
 * any message in the terminal.
 */
const char* status_colors[3] = {"\033[32m", "\033[31m", "\033[33m"};

u8 PrintMessage(u8 state, char* message, ...)
{
    // If the terminal's width hasn't been grabbed yet, do it. If this function
    // fails the whole program is sent to hell, so we don't bother error
    // checking.
    if (terminal_width == 0) GetTerminalWidth();
    // Similarly, if the program's start time hasn't yet been grabbed, do that.
    // This function will, on first call, always return zero, so we cast it to
    // void as we do not care.
    if (start_time == 0) (void)GetCurrentTime();

    u64 line_length = 0, last_assigned_length = 0;

    if (state > (sizeof status_strings) / (sizeof status_strings[0]))
        return FAILURE;

    // Print the message status to the console, using the given color and char*
    // label.
    line_length = printf("%s[%s]  MESSAGE:\033[0m ", status_colors[state],
                         status_strings[state]);
    if (line_length < 0) return FAILURE;

    // Get the variadic arguments passed into the function. If none are passed,
    // this is void.
    va_list args;
    va_start(args, message);

    // Calculate the lenght of the char*, print the formatted message, and
    // check if we need to fail the function. If not, add in the new length.
    last_assigned_length = vprintf(message, args);
    if (last_assigned_length < 0) return FAILURE;
    line_length += last_assigned_length;

    // Set the global Last_time_string value to, well, now.
    GetTimeString();

    // Do possibly the most ugly operation ever (and probably the most
    // inefficient one too!) and append the time onto the console's output. Use
    // the substring functionality of printf to only print the spaces we need,
    // and then the time value.
    if (printf(
            "%.*s%sTIME:\033[0m %s\n",
            (i32)(terminal_width - line_length - strlen(last_time_string) + 3),
            "                                                               "
            "                                                               "
            "                                                               ",
            status_colors[state], last_time_string) < 0)
        return FAILURE;

    // Return that we've hit no issues.
    return SUCCESS;
}

// If debug mode is not on, just have the PrintMessage function do nothing.
#else
u8 PrintMessage(u8 state, char* message, ...) {}
#endif // DEBUG_MODE
