#include "Logger.h" // Provides the logger mother header.
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

/**
 * @brief Get a message to go along with whatever status we've been passed. This
 * is always a string.
 */
#define STATUS_MESSAGE(type)                                                   \
    (type == success ? "\033[32m[  SUCCESS  ]"                                 \
     : type == error ? "\033[31m[   ERROR   ]"                                 \
                     : "\033[33m[  WARNING  ]")
/**
 * @brief Get a color to go along with whatever status we've been passed. This
 * is always an ANSI color escape code.
 */
#define COLOR_MESSAGE(type)                                                    \
    (type == success ? "\033[32m" : type == error ? "\033[31m" : "\033[33m")

__KILLFAIL PrintMessage(MessageState state, const char* caller, char* message,
                        ...)
{
    // If the terminal's width hasn't been grabbed yet, do it. If this function
    // fails the whole program is sent to hell, so we don't bother error
    // checking.
    if (!terminal_width) _GetTerminalWidth();
    // Similarly, if the program's start time hasn't yet been grabbed, do that.
    // This function will, on first call, always return zero, so we cast it to
    // void as we explicitly do not care about this value.
    if (!start_time) (void)GetCurrentTime();

    // A variable to store the total line length printed thus far, for some
    // space-printing calculations at the end.
    u32 line_length = 0;

    // Print the message status to the console, using the given color and string
    // label. If this, for some reason, fails, print the error and fail the
    // process.
    if ((line_length = printf("%s MESSAGE:\033[0m ", STATUS_MESSAGE(state))) <
        0)
    {
        PrintError(
            "An input/output error has occurred. Please report this bug ASAP.");
        exit(-1);
    }

    // Collect the variable arguments. If there are none provided, this is
    // simply initialized to NULL and each function that uses it will ignore it.
    va_list args;
    va_start(args, message);

    // Try to print out a string formatted with the variable args. If this, for
    // some reason, fails, kill the process.
    u32 variable_arg_print_length = 0;
    if ((variable_arg_print_length = vprintf(message, args)) < 0)
    {
        PrintError("An input/output error has occurred. Please report this bug "
                   "ASAP. ");
        exit(-1);
    }
    // Add the amount of characters we just printed to the line length.
    line_length += variable_arg_print_length;

    // Get the time string. Note that we define its max length as the terminal
    // width minus the length of whatever we've already printed.
    char time_string[terminal_width - line_length];
    GetTimeString(time_string, terminal_width - line_length);
    // Create the final "line length", or really, the distance we will need to
    // go until we print the time string on the right side of the output.
    line_length = terminal_width - line_length - strlen(time_string) + 7;

    // Try to print the time string. If this, for some ungodly reason, fails,
    // kill the process in a panic.
    if (printf("%*sTIME:\033[0m %s\n", line_length, COLOR_MESSAGE(state),
               time_string) < 0)
    {
        PrintError(
            "An input/output error has occurred. Please report this bug ASAP.");
        exit(-1);
    }
}
#else
u8 PrintMessage(u8 state, char* message, ...) {}
#endif

#ifdef linux
#define error_reporter 0
#else
#define error_reporter 1
#endif

void copy(i32 fd, i16 ev, void* arg) { printf("e"); }

__KILL PrintErrorMessage(const char* caller, i32 line, char* message, ...)
{
    char msg[512];
    snprintf(msg, 512,
             "notify-send -u critical -a Renai -t 0 \"Renai Error Reporter\" "
             "\"Renai ran into an error. Caller: %s on line %d. Message: '",
             caller, line);

    va_list args;
    va_start(args, message);
    vsnprintf(msg + strlen(msg), 512 - strlen(msg), message, args);
    strncat(msg, "'\"", 512 - strlen(msg));

    if (error_reporter == 0) system(msg);
    else
    {
        //! windows error reporter!!
    }

    exit(-1);
}
