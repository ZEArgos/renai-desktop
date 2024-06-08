/**
 * @file Declarations.h
 * @author Zenais Argos
 * @brief File to define various macros, functions, and global program variables
 * in a centralized, traceable location.
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_DECLARATIONS_
#define _RENAI_DECLARATIONS_

// Provides a list of each include needed in the various translation units of
// the project. IncludeBoard.h exists simply because it's ugly to have its
// contents here.
#include <IncludeBoard.h>

// (-9,223,372,036,854,775,807, +9,223,372,036,854,775,807)
typedef int64_t i64;
// (−2,147,483,647, +2,147,483,647)
typedef int32_t i32;
// (−32,767, +32,767)
typedef int16_t i16;
// (−127, +127)
typedef int8_t i8;
// (0, +18,446,744,073,709,551,615)
typedef uint64_t u64;
// (0, +4,294,967,295)
typedef uint32_t u32;
// (0, +65,535)
typedef uint16_t u16;
// (0, +255)
typedef uint8_t u8;
// (+/-3.4028235 × 10^38)
typedef float f32;
// (to my knowledge, +/-1.79769313486e+308)
typedef double f64;
// (to my knowledge, +/-1.18973149535723176502e+4932)
typedef long double f128;

/**
 * @brief Syntactic sugar for the value 'true.'
 */
#define SUCCESS 1
/**
 * @brief Syntactic sugar for the value 'false.'
 */
#define FAILURE 0

/**
 * @brief Indicates a function that kills the application on fail. This is
 * simply a distinguisher between actual void-return functions and error-kill
 * functions.
 */
#define __KILLFAIL void
/**
 * @brief Indicates a function that will @b always kill the process on
 * completion. This is used nearly exclusively by the error-handling interface.
 */
#define __KILL void

/**
 * @brief Get a string representation of the current time in milliseconds.
 * @param storage The string which will become the host of the current time.
 * @param string_size The size of the given string.
 */
void GetTimeString(char* storage, u32 string_size);

/**
 * @brief The exact amount of milliseconds it had been since the beginning of
 * the epoch when the application began.
 */
extern i64 start_time;

/**
 * @brief Get the current time, based off of @ref start_time and the @b current
 * number of milliseconds it's been since the beginning of the epoch.
 * @return A 64-bit integer representation of the milliseconds difference
 * between the two; the amount of time the application has been running.
 */
i64 GetCurrentTime(void);

/**
 * @brief Count the number of digits in the number passed to the function. Note
 * that this function's effectiveness cuts at 255 digits, as the situation I use
 * this function in does not necessitate any longer numbers.
 * @param number The number whose digits we want to count.
 * @return A 16-bit unsigned integer that represents the size of the number, or
 * 0 if something went wrong.
 */
u8 CountDigits(u32 number);

/**
 * @brief Get a string representation of the current time and date, and
 * concatenate it into the given string buffer.
 * @param buffer The buffer into which we're copying the time and date.
 */
void GetDateString(char* buffer);

/**
 * @brief Look for any potential GLFW errors. If none are found, simply finish
 * the function with a success code.
 * @return An 8-bit integer flag representing success state. The function fails
 * if it finds an error.
 */
u8 PrintGLFWError(void);

/**
 * @brief Look for any potential OpenGL errors. If no errors are found, finish
 * the function.
 * @return An 8-bit integer flag representing success state. The function fails
 * if it finds and error.
 */
u8 PrintGLError(void);

#endif // _RENAI_DECLARATIONS_
