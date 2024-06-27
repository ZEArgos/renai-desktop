#include "Logger.h"
#include <stdarg.h>

// Most of the code in this file is highly distribution-dependent,
// meaning that if the application was compiled in debug mode, the
// method does one thing, but it does something completely separate in
// production. For that reason, we define a section for debug-only
// code and production-only code.
#ifdef DEBUG_MODE

/**
 * @brief A boolean value that tells the application if its start time
 * has been recorded or not.
 */
bool start_time_initialized = false;

__KILLFAIL PrintMessage(u8 state, const char* caller, char* message,
                        ...)
{
    if (!start_time_initialized)
    {
        (void)GetCurrentTime();
        start_time_initialized = true;
    }

    // Print the beginning of the message. The main purpose of this
    // call is to setup the color.
    printf("\n%s", (state == 1 ? "\033[32m[ " : "\033[33m[ "));

    char log_time[11];
    GetTimeString(log_time, 11);

    printf("%s ]\033[0m ", log_time);

    // Grab the variable arguments of the function call.
    va_list args;
    va_start(args, message);
    // Print out the formatted message with the arguments.
    vprintf(message, args);
}

#endif

__KILL PrintErrorMessage(const char* caller_parent,
                         const char* caller, i32 line, char* message,
                         ...)
{
    char msg[512];
    // Do the first string concatenation into the buffer, this one
    // with the most of the meat. If this fails, the user must know at
    // all costs, so we'll try to directly talk to them. If THAT,
    // dystopically, fails, well, their computer is probably fucked,
    // so we'll just ignore it.
    snprintf(msg, 512,
             "notify-send -u critical -a Renai -t 0 \"Renai Error "
             "Reporter\" "
             "\"Renai ran into an error. Caller: %s > %s on line %d. "
             "Message: '",
             caller_parent, caller, line);

    // Get the variable arguments passed to the function, so we can
    // utilize them in the vsnprintf call.
    va_list args;
    va_start(args, message);
    vsnprintf(msg + strlen(msg), 512 - strlen(msg), message, args);
    (void)strncat(msg, "'\"", 512 - strlen(msg));

    // Syscall the command. This will execute the
    // notify-send. We...I...cannot fix any errors associated with
    // this function, so we just explicitly ignore them.
    (void)system(msg);

    exit(-1);
}
