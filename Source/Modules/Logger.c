#include "Logger.h" // Provides the logger mother header.
#include <stdarg.h> // Provides the standard C library variable argument method module.

// Most of the code in this file is highly distribution-dependent, meaning that
// if the application was compiled in debug mode, the method does one thing, but
// it does something completely separate in production. For that reason, we
// define a section for debug-only code and production-only code.
#ifdef DEBUG_MODE

/**
 * @brief A boolean value that tells the application if its start time has been
 * recorded or not.
 */
bool start_time_initialized = false;

__KILLFAIL PrintMessage(u8 state, const char* caller, char* message, ...)
{
    // Similarly, if the program's start time hasn't yet been grabbed, do that.
    // This function will, on first call, always return zero, so we cast it to
    // void as we explicitly do not care about this value.
    if (!start_time_initialized)
    {
        (void)GetCurrentTime();
        start_time_initialized = true;
    }

    // Print the beginning of the message. The main purpose of this call is to
    // setup the color.
    if (printf("\n%s", (state == 1 ? "\033[32m[ " : "\033[33m[ ")) < 0)
        PrintError("An output error occurred. (1)");

    // Allocate a buffer of 11 characters to hold the 10-character time string
    // and its null terminator.
    char* log_time = malloc(11);
    if (log_time == NULL)
        PrintError("Failed to allocate space for the log string.");
    // Get the 10-character time string.
    GetTimeString(log_time);

    // Print it out, and make sure to close the color.
    if (printf("%s ]\033[0m ", log_time) < 0)
        PrintError("An output error occurred. (2)");

    // Grab the variable arguments of the function call.
    va_list args;
    va_start(args, message);
    // Print out the formatted message with the arguments.
    if (vprintf(message, args) < 0) PrintError("An output error occurred. (3)");
}

#endif

__KILL PrintErrorMessage(const char* caller_parent, const char* caller,
                         i32 line, char* message, ...)
{
    // Allocate enough space for the message we are going to print. This should
    // not exceed 512 characters, so we set a hard limit there.
    char msg[512];
    // Do the first string concatenation into the buffer, this one with the most
    // of the meat. If this fails, the user must know at all costs, so we'll try
    // to directly talk to them. If THAT, dystopically, fails, well, their
    // computer is probably fucked, so we'll just ignore it.
    if (snprintf(
            msg, 512,
            "notify-send -u critical -a Renai -t 0 \"Renai Error Reporter\" "
            "\"Renai ran into an error. Caller: %s > %s on line %d. Message: '",
            caller_parent, caller, line) < 0)
    {
        (void)system(
            "notify-send -u critical -a Renai -t 0 \"Renai Error Reporter\" "
            "\"The Renai error reporter failed. Something is either seriously "
            "wrong with your computer, or this program. Please report ASAP.\"");
        exit(-1);
    }

    // Get the variable arguments passed to the function, so we can utilize
    // them in the vsprintf.
    va_list args;
    va_start(args, message);
    // Actually put the messaage into the buffer object, making certain not to
    // overflow the buffer in case of a particularly large buffer; instead the
    // command will just fail on execution. Just like the earlier snprintf call,
    // if this, somehow, fails, we're screwed.
    if (vsnprintf(msg + strlen(msg), 512 - strlen(msg), message, args) < 0)
    {
        (void)system(
            "notify-send -u critical -a Renai -t 0 \"Renai Error Reporter\" "
            "\"The Renai error reporter failed. Something is either seriously "
            "wrong with your computer, or this program. Please report ASAP.\"");
        exit(-1);
    }
    (void)strncat(msg, "'\"", 512 - strlen(msg));

    // If we're on linux, just syscall the command. This will execute the
    // notify-send. We...cannot fix any errors associated with this function, so
    // we just explicitly ignore them.
    (void)system(msg);

    // Kill the application with an error.
    exit(-1);
}
