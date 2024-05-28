/**
 * @file Declarations.h
 * @author Zenais Argos
 * @brief File to define various macros, functions, and global program variables
 * in a centralized, traceable location.
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 */

// Include guard the file so its contents are not inserted into the final binary
// twice. This also prevents undue compiler errors.
#ifndef _RENAI_DECLARATIONS_
#define _RENAI_DECLARATIONS_

// Cross-compilation includes that are different on Windows vs. Linux go here,
// so the compiler can insert them as needed.
#if defined(linux)
/**
 * @file bits/stdint-intn.h
 * @brief Provides the various integer type defines that make my
 * cross-compilation job much, much easier.
 * @see The C standard library/manual
 */
#include <bits/stdint-intn.h>
/**
 * @file bits/stdint-uintn.h
 * @brief Provides the various unsigned integer type defines that make my
 * cross-compilation job much, much easier.
 * @see The C standard library/manual
 */
#include <bits/stdint-uintn.h>
#elif defined(_WIN32)
// insert cool win32 includes here
#else
#error "Compiling on unsupported operating system. Please reconsider."
#endif

/**
 * @file errno.h
 * @brief Provides the ERRNO global variable, for diagnosing problems with
 * system calls.
 * @see The C standard library/manual
 */
#include <errno.h>
/**
 * @file stdio.h
 * @brief Provides all C input/output related functionality.
 * @see The C standard library/manual
 */
#include <stdio.h>
/**
 * @file stdlib.h
 * @brief Provides a BUNCH of C standard functionality, pretty much a given in
 * any C program.
 * @see The C standard library/manual
 */
#include <stdlib.h>
/**
 * @file stdlib.h
 * @brief Provides a ton of C standard string functionality, another typical
 * given for many C programs.
 * @see The C standard library/manual
 */
#include <string.h>
/**
 * @file glad.h
 * @brief The file that defines GLAD, my OpenGL wrangler of choice. There's
 * really nothing special about it, I just know how to use it, kinda.
 * @see An OpenGL documentation website of your choice (https://docs.gl)
 */
#include <glad/glad.h>
/**
 * @file glfw3.h
 * @brief The file that defines the GLFW library, which does all the heavy
 * lifting for me on cross-compatible window creation + management. I love GLFW.
 * @see The GLFW documentation (https://www.glfw.org/documentation.html)
 */
#include <glfw/glfw3.h>

// Syntactic sugar for the various type cross-compatibile type defines provided
// by the C standard library. I find them a little...verbose, so I choose to
// change them.

// x64 integer (-9,223,372,036,854,775,807, +9,223,372,036,854,775,807)
typedef int64_t i64;
// x32 integer (−2,147,483,647, +2,147,483,647)
typedef int32_t i32;
// x16 integer (−32,767, +32,767)
typedef int16_t i16;
// x8 integer (−127, +127)
typedef int8_t i8;
// x64 unsigned integer (0, +18,446,744,073,709,551,615)
typedef uint64_t u64;
// x32 unsigned integer (0, +4,294,967,295)
typedef uint32_t u32;
// x16 unsigned integer (0, +65,535)
typedef uint16_t u16;
// x8 unsigned integer (0, +255)
typedef uint8_t u8;

// x32 floating point (+/-3.4028235 × 10^38)
typedef float f32;
// x64 floating point (to my knowledge, +/-1.79769313486e+308)
typedef double f64;
// ~x128 floating point (to my knowledge, +/-1.18973149535723176502e+4932)
typedef long double f128;

// pointer to character array
typedef char* string;
// constant pointer to character array
typedef const char* cstring;
// nothing
typedef void null;

#define SUCCESS 1
#define FAILURE 0

extern u16 terminal_width;
null GetTerminalWidth(void);

extern i64 start_time;
i64 GetCurrentTime(void);

#define MAX_TIME_STRING_LENGTH 256
extern char last_time_string[MAX_TIME_STRING_LENGTH];
null GetTimeString(void);
null FormatTimeString(i64 ms, i64 s, i64 m);

u8 PrintOpenGLError(void);

#endif // _RENAI_DECLARATIONS_
