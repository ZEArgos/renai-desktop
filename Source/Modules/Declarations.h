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
 * @brief Indicates a function that kills the process on fail. This is
 * simply a distinguisher between actual void-return functions and error-kill
 * functions.
 */
#define __KILLFAIL void
/**
 * @brief Indicates a function that will @b always kill the process on
 * completion. This is used nearly exclusively by the error-handling interface.
 */
#define __KILL _Noreturn void
/**
 * @brief Indicates that the marked function only returns 1 or 0.
 */
#define __BOOLEAN _Bool
/**
 * @brief Indicates that the marked function makes use of a caller-provided
 * buffer object, and as such does not return anything.
 */
#define __PROVIDEDBUFFER void
/**
 * @brief Indicates that the function returns an ambiguously-types value.
 */
#define __AMBIGUOUS void*
/**
 * @brief Indicates a function that creates a structure.
 */
#define __CREATE_STRUCT(structure) structure*
/**
 * @brief Indicates a function that creates a structure but kills the process on
 * fialure.
 */
#define __CREATE_STRUCT_KILLFAIL(structure) structure*
/**
 * @brief Indicates a structure getter function.
 */
#define __GET_STRUCT(name) name*
/**
 * @brief Indicates an inline function.
 */
#define __INLINE static inline

/**
 * @brief Convert a void pointer to the given type.
 */
#define VPTT(type, value) *((type*)value)
/**
 * @brief Convert the given value to a void pointer.
 */
#define TTVP(value) (void*)&value

/**
 * @brief A function to poll the application's runtime for any GLFW errors.
 * @param caller The caller of the function.
 */
__KILLFAIL PollGLFWErrors(const char* caller);

/**
 * @brief A function to poll the application's runtime for any OpenGL errors.
 * @param caller The caller of the function.
 */
__KILLFAIL PollOpenGLErrors(const char* caller);

/**
 * @brief Get a string representation of the current time in milliseconds.
 * @param buffer The string which will become the host of the current time.
 */
__PROVIDEDBUFFER GetTimeString(char* buffer, u8 buffer_length);

/**
 * @brief Get a string representation of the current time and date, and
 * concatenate it into the given string buffer.
 * @param buffer The buffer into which we're copying the time and date.
 * @param buffer_length The length of the buffer provided.
 */
__PROVIDEDBUFFER GetDateString(char* buffer, u8 buffer_length);

/**
 * @brief Get the current time, based off of @ref start_time and the @b current
 * number of milliseconds it's been since the beginning of the epoch.
 * @return A 64-bit integer representation of the milliseconds difference
 * between the two; the amount of time the application has been running.
 */
i64 GetCurrentTime(void);

/**
 * @brief Calculate how well the application is running, in regards to how many
 * frames it @b could render in a second. Note the 'possible' in this function's
 * name. That is intentional, since the application is clamped to run at an
 * arbitrary VSYNC value as specified by @ref ChangeApplicationFrameCap.
 * @param new_value The newest time it's taken to render a frame.
 * @return The possible framerate.
 */
f64 CalculatePossibleFramerate(u32 new_value);

/**
 * @brief Count the number of digits in the number passed to the function. Note
 * that this function's effectiveness cuts at 255 digits, as the situation I use
 * this function in does not necessitate any longer numbers.
 * @param number The number whose digits we want to count.
 * @return A 16-bit unsigned integer that represents the size of the number, or
 * 0 if something went wrong.
 */
u16 CountDigits(u32 number);

#endif // _RENAI_DECLARATIONS_
