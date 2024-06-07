#include "Logger.h"       // Provides the logger mother header.
#include "Declarations.h" // The Declarations header file, used in this context exclusively for its type definitions.
#include <stdarg.h> // Provides the standard C library variable argument method module.

// Most of the code in this file is highly distribution-dependent, meaning that
// if the application was compiled in debug mode, the method does one thing, but
// it does something completely separate in production. For that reason, we
// define a section for debug-only code and production-only code.
#ifdef DEBUG_MODE

// Provides the IO control tool, which allows the program to poll the operating
// system for information.
#include <sys/ioctl.h>
// Provides the Linux time methods.
#include <sys/time.h>
// Provides the Unix symbol constant definitions, such as various file
// definitions and some methods to grab information about the process.
#include <unistd.h>

/**
 * @brief The terminal width of the application, set with the @ref
 * _GetTerminalWidth method.
 */
u16 terminal_width = 0;

/**
 * @brief Get the width of the currently running terminal for the application.
 * Note that this function will only do something once, after that it just
 * returns.
 */
void _GetTerminalWidth(void)
{
    // Check to make sure this function has not been called already, and if it
    // has, return instantly.
    if (terminal_width != 0) return;

    // Setup a place to store the various terminal information.
    struct winsize terminal_dimensions;
    // Use the IO control tool to poll the terminal's dimensions, and if it
    // fails, print to the current output and exit the process with an exit
    // code.
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_dimensions) == -1)
    {
        PrintError("Failed to get terminal dimensions, code: %d.", errno);
        exit(-1);
    }
    // Set the terminal width to the polled value, discarding the rest.
    terminal_width = terminal_dimensions.ws_col;
}

#define SUCCESS_STATUS "\033[32m[  SUCCESS  ]"
#define ERROR_STATUS   "\033[31m[   ERROR   ]"
#define WARNING_STATUS "\033[33m[  WARNING  ]"

void PrintMessage(MessageState state, char* message, ...)
{
    // If the terminal's width hasn't been grabbed yet, do it. If this function
    // fails the whole program is sent to hell, so we don't bother error
    // checking.
    if (terminal_width == 0) _GetTerminalWidth();
    // Similarly, if the program's start time hasn't yet been grabbed, do that.
    // This function will, on first call, always return zero, so we cast it to
    // void as we do not care.
    if (start_time == 0) (void)GetCurrentTime();

    u64 line_length = 0, last_assigned_length = 0;

    // Print the message status to the console, using the given color and char*
    // label.
    line_length =
        printf("%s MESSAGE:\033[0m ", (state == success ? SUCCESS_STATUS
                                       : state == error ? ERROR_STATUS
                                                        : WARNING_STATUS));
    if (line_length < 0) return; //!!!

    // Get the variadic arguments passed into the function. If none are passed,
    // this is void.
    va_list args;
    va_start(args, message);

    // Calculate the lenght of the char*, print the formatted message, and
    // check if we need to fail the function. If not, add in the new length.
    last_assigned_length = vprintf(message, args);
    if (last_assigned_length < 0) return; //!!!
    line_length += last_assigned_length;

    // Set the global Last_time_string value to, well, now.
    char time_string[512];
    GetTimeString(time_string, 512);

    // Do possibly the most ugly operation ever (and probably the most
    // inefficient one too!) and append the time onto the console's output. Use
    // the substring functionality of printf to only print the spaces we need,
    // and then the time value.
    if (printf(
            "%.*s%sTIME:\033[0m %s\n",
            (i32)(terminal_width - line_length - strlen(time_string) + 3),
            "                                                               "
            "                                                               "
            "                                                               ",
            (state == success ? "\033[32m"
             : state == error ? "\033[31m"
                              : "\033[33m"),
            time_string) < 0)
        return; //!!

    // Return that we've hit no issues.
    return; //!!
}
#else
u8 PrintMessage(u8 state, char* message, ...)
{
    char msg[512] = "notify-send -t 0 'Renai ran into an error: ";
    strncat(msg, message, 485);
    strncat(msg, "'", 486);

    char msg_full[512];
    va_list args;
    va_start(args, message);
    vsprintf(msg_full, msg, args);

    system(msg_full);
    return SUCCESS;
}
#endif
