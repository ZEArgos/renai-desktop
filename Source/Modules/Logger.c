#include "Logger.h"
#include "Declarations.h"

#ifdef DEBUG_MODE

// Provides variadic argument handling functionality.
#include <stdarg.h>

/**
 * @brief A constant array of string to specify the messages given to the
 * terminal when a specific status is called upon.
 */
const char* status_strings[2] = {"  SUCCESS  ", "   ERROR   "};
/**
 * @brief A constant array of color escape codes to indicate the status of
 * any message in the terminal.
 */
const char* status_colors[2] = {"\033[32m", "\033[31m"};

u8 PrintMessage(u8 state, string message, ...)
{
    // If the terminal's width hasn't been grabbed yet, do it. If this function
    // fails the whole program is sent to hell, so we don't bother error
    // checking.
    if (terminal_width == 0)
        GetTerminalWidth();
    // Similarly, if the program's start time hasn't yet been grabbed, do that.
    // This function will, on first call, always return zero, so we cast it to
    // void as we do not care.
    if (start_time == 0)
        (null) GetCurrentTime();

    u64 line_length = 0, last_assigned_length = 0;

    // Print the message status to the console, using the given color and string
    // label.
    line_length = printf("%s[%s]\033[0m\033[33m  MESSAGE:\033[0m ",
                         status_colors[state], status_strings[state]);
    if (line_length < 0)
        return FAILURE;

    // Get the variadic arguments passed into the function. If none are passed,
    // this is null.
    va_list args;
    va_start(args, message);

    // Calculate the lenght of the string, print the formatted message, and
    // check if we need to fail the function. If not, add in the new length.
    last_assigned_length = vprintf(message, args);
    if (last_assigned_length < 0)
        return FAILURE;
    line_length += last_assigned_length;

    // Set the global Last_time_string value to, well, now.
    GetTimeString();

    // Do possibly the most ugly operation ever (and probably most inefficient
    // one too!) and append the time onto the console's output. Use the
    // substring functionality of printf to only print the spaces we need, and
    // then the time value.
    if (printf(
            "%.*s\033[33mTIME:\033[0m %s\n",
            (i32)(terminal_width - line_length - strlen(last_time_string) + 7),
            "                                                               "
            "                                                               "
            "                                                               ",
            last_time_string) < 0)
        return FAILURE;

    // Return that we've hit no issues.
    return SUCCESS;
}

// If debug mode is not on, just have the PrintMessage function do nothing.
#else
u8 PrintMessage(u8 state, string message, ...) { return SUCCESS; }
#endif // DEBUG_MODE
