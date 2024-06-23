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
 * @brief Syntactic sugar to create a struct. All this really does is prevent me
 * from having to write "typedef" every time I need to create a struct; I find
 * it @b really annoying.
 */
#define __STRUCT(name, contents) typedef struct name contents name
/**
 * @brief Much the same as @ref __STRUCT, syntactic sugar to create a union,
 * mostly letting me forget about having to type "typedef" every union.
 */
#define __UNION(name, contents) typedef union name contents name
/**
 * @brief Much the same as @ref __STRUCT & @ref __UNION, syntactic sugar to
 * create an enum, mostly letting me forget about having to type "typedef" every
 * enum.
 */
#define __ENUM(name, ...) typedef enum name __VA_ARGS__ name

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
 * @brief The types possible of for an ambiguous type to be (u32, u64, i32,
 * i64).
 */
__ENUM(AmbiguousTypeSpecifier, {unsigned32, unsigned64, signed32, signed64});

/**
 * @brief An ambiguous type. Its constraints are described by the @ref
 * AmbiguousTypeSpecifier enum.
 */
__UNION(AmbiguousType, {
    u32 unsigned32;
    u64 unsigned64;
    i32 signed32;
    i64 signed64;
});

/**
 * @brief A macro used to get the enum typename of a signed value.
 */
#define __TYPENAME_i(size) signed##size
/**
 * @brief The sister function of @ref __TYPENAME_i, a macro used to get
 * the enum typename of an unsigned value.
 */
#define __TYPENAME_u(size) unsigned##size
/**
 * @brief A macro to get the field of an ambiguous type.
 */
#define __FIELD(state, size) __TYPENAME_##state(size)

/**
 * @brief The body of an ambiguous function. Defined here purely to remove the
 * repetition of having to type this expression out for each ambiguous function.
 */
#define __AMBIGUOUS_BODY(variable, expression)                                 \
    switch (variable)                                                          \
    {                                                                          \
        case __FIELD(u, 32): expression(u, 32) break;                          \
        case __FIELD(i, 32): expression(i, 32) break;                          \
        case __FIELD(u, 64): expression(u, 64) break;                          \
        case __FIELD(i, 64): expression(i, 64) break;                          \
    }

/**
 * @brief Assign a value to an ambiguous type.
 * @param affected The affected variable.
 * @param member What state are we making the ambiguous type?
 * @param value The value that state will have.
 */
void AssignAmbiguousType(AmbiguousType* affected, AmbiguousTypeSpecifier member,
                         void* value);

/**
 * @brief Get the current value of the specified ambiguous type.
 * @param affected The ambiguous type to grab from.
 * @param member The member of the type we're trying to get.
 * @return The value of the member.
 */
__AMBIGUOUS GetAmbiguousType(AmbiguousType* affected,
                             AmbiguousTypeSpecifier member);

/**
 * @brief A helper function to compare two ambiguous types.
 * @param affected The affected ambiguous type.
 * @param member The member of the ambiguous type we're accessing.
 * @param value The value to compare against.
 * @return A boolean value that represents the state of the comparison; true for
 * similarity, false for difference.
 */
__BOOLEAN CompareAmbiguousType(AmbiguousType* affected,
                               AmbiguousTypeSpecifier member, void* value);

/**
 * @brief Get a string representation of the current time in milliseconds. The
 * output string will always be 11 characters (including the '\0'), it is up to
 * the caller to make certain the string can hold that.
 * @param storage The string which will become the host of the current time.
 */
__PROVIDEDBUFFER GetTimeString(char* storage);

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
u16 CountDigits(u32 number);

//!
f64 CalculateFramerate(u32 new_value);

/**
 * @brief Get a string representation of the current time and date, and
 * concatenate it into the given string buffer.
 * @param buffer The buffer into which we're copying the time and date.
 */
__PROVIDEDBUFFER GetDateString(char* buffer);

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

#endif // _RENAI_DECLARATIONS_
