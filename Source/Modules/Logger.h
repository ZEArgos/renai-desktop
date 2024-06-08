/**
 * @file Logger.h
 * @author Zenais Argos
 * @brief File to declare functions and various other utilities to be used when
 * logging messages to the console. This should be available in very nearly
 * every translation unit throughout the project.
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 */

// Include guard since this file is needed in many parts of the program.
#ifndef _RENAI_LOGGER_
#define _RENAI_LOGGER_

// The Declarations header file, used in this context exclusively for its type
// definitions.
#include "Declarations.h"

typedef enum MessageState
{
    success,
    error,
    warning
} MessageState;

/**
 * @brief Print a message to whatever output the application is currently
 * set to utilize. In debug mode, that would be the standard output, and in
 * production binaries, that would be the @ref notify-send or @ref
 * MessageBox interface, depending on the operating system.
 * @param state The state of the message.
 * @param message The message to send. This is similar to the format string
 * in any of the @ref printf function family.
 * @param ... The arguments to concatenate into @param message.
 */
__KILLFAIL PrintMessage(MessageState state, const char* caller, char* message,
                        ...);

__KILL PrintErrorMessage(const char* caller, i32 line, char* message, ...);

#define PrintSuccess(...) PrintMessage(success, __func__, __VA_ARGS__)
#define PrintError(...)   PrintErrorMessage(__func__, __LINE__, __VA_ARGS__)
#define PrintWarning(...) PrintMessage(warning, __func__, __VA_ARGS__)

#ifndef DEBUG_MODE
#undef PrintSuccess(...)
#undef PrintWarning(...)
#endif

#endif // _RENAI_LOGGER_
