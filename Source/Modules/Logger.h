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

// As to prevent unnecessary bloat--however small it may be--inside the
// production binaries, add/remove this code althogether depending on the
// application's state.
#ifdef DEBUG_MODE

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
__KILLFAIL PrintMessage(u8 state, const char* caller, char* message, ...);

/**
 * @brief Print A specifically success message, which is colored in green.
 */
#define PrintSuccess(...) PrintMessage(1, __func__, __VA_ARGS__)
/**
 * @brief Print a warning message to the standard terminal. This message will be
 * colored yellow.
 */
#define PrintWarning(...) PrintMessage(0, __func__, __VA_ARGS__)

#else
#define PrintSuccess(...)
#define PrintWarning(...)
#endif

/**
 * @brief Print an error message to the secondary output of the application, as
 * to be certain the message gets out if something goes wrong. This functions @b
 * always kills the process.
 * @param caller The caller name of the function that calls the method.
 * @param line The line number the call came from.
 * @param message The message to be printed.
 * @param ... Any variable arguments to be concatenated into @param message.
 */
__KILL PrintErrorMessage(const char* caller_parent, const char* caller,
                         i32 line, char* message, ...);

/**
 * @brief Print an error to the secondary output. This macro makes calling the
 * function a whole lot easier and less cumbersome.
 */
#define PrintError(...)                                                        \
    PrintErrorMessage(caller, __func__, __LINE__, __VA_ARGS__)

#endif // _RENAI_LOGGER_
