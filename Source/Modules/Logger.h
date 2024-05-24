/**
 * @file Logger.h
 * @author Zenais Argos
 * @brief File to declare functions and various other utilities to be used when logging messages to the
 * console. This should be available in every translation unit throughout the project.
 * @date 2024-05-23
 * 
 * @copyright Copyright (c) 2024
*/

// Include guard since this file is needed in many parts of the program.
#ifndef _RENAI_LOGGER_
#define _RENAI_LOGGER_

// Include for my various typedefs.
#include "Declarations.h"

u8 PrintMessage(string task_msg, ...);
u8 PrintError(string task_msg, ...);

#endif // _RENAI_LOGGER_
