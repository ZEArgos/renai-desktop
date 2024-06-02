/**
 * @file Logger.h
 * @author Zenais Argos
 * @brief File to declare functions and various other utilities to be used when
 * logging messages to the console. This should be available in every
 * translation unit throughout the project.
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 */

// Include guard since this file is needed in many parts of the program.
#ifndef _RENAI_LOGGER_
#define _RENAI_LOGGER_

// Include for my various typedefs.
#include "Declarations.h"

/**
 * @brief Print a formatted and colored message to the console. Works basically
 * as a wrapper around @ref printf() in the way it takes variadic args and
 * concatenates them into the given message.
 * @param state The state in which to print. (i.e failure/success)
 * @param message The message given to print, with optional formatting
 * characters to concatenate string into.
 * @param ... The various arguments to be concatenated into the main message.
 * @return An unsigned, 8-bit integer flag representing either success or
 * failure.
 */
u8 PrintMessage(u8 state, string message, ...);

#ifdef DEBUG_MODE
/**
 * @brief Print a message in success mode.
 */
#define PrintSuccess(...) PrintMessage(0, __VA_ARGS__)
/**
 * @brief Print a message in warning mode.
 */
#define PrintWarning(...) PrintMessage(2, __VA_ARGS__)
#else
/**
 * @brief Print a message in success mode. (REMOVED DUE TO NON-DEBUG MODE)
 */
#define PrintSuccess(...)
/**
 * @brief Print a message in warning mode. (REMOVED DUE TO NON-DEBUG MODE)
 */
#define PrintWarning(...)
#endif

/**
 * @brief Print a message in error mode.
 */
#define PrintError(...) PrintMessage(1, __VA_ARGS__)

#endif // _RENAI_LOGGER_
