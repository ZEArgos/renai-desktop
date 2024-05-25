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

// Syntactic sugar for the various type cross-compatibile type defines provided
// by the C standard library. I find them a little...verbose, so I choose to
// change them.

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef float f32;
typedef double f64;
// Note that this doesn't necessarily have to be 128 bits, I just say this
// because it fits the scheme and is possible.
typedef long double f128;

typedef char* string;
typedef void null;

#define SUCCESS 0
#define FAILURE 1

extern u16 terminal_width;
null GetTerminalWidth(void);

extern i64 start_time;
i64 GetCurrentTime(void);
char* GetTimeString(void);

#endif // _RENAI_DECLARATIONS_
