/**
 * @file Ambiguous.h
 * @author Zenais Argos
 * @brief Provides the data structure and helper functions to deal
 * with ambiguous data types.
 * @date 2024-06-25
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_AMBIGUOUS_
#define _RENAI_AMBIGUOUS_

// We use data types and syntactic sugar from this file to define
// things here.
#include <Declarations.h>

/**
 * @brief The types possible of for an ambiguous type to be (u8, u32,
 * u64, i8, i32, i64).
 */
typedef enum AmbiguousTypeSpecifier
{
    unsigned8,
    unsigned32,
    unsigned64,
    signed8,
    signed32,
    signed64
} AmbiguousTypeSpecifier;

/**
 * @brief An ambiguous type. Its constraints are described by the @ref
 * AmbiguousTypeSpecifier enum.
 */
typedef union AmbiguousType
{
    u8 unsigned8;
    u32 unsigned32;
    u64 unsigned64;
    i8 signed8;
    i32 signed32;
    i64 signed64;
} AmbiguousType;

/**
 * @brief The body of an ambiguous function. Defined here purely to
 * remove the repetition of having to type this expression out for
 * each ambiguous function.
 */
#define __AMBIGUOUS_BODY(variable, expression)                       \
    switch (variable)                                                \
    {                                                                \
        case __FIELD(u, 8):  expression(u, 8) break;                 \
        case __FIELD(i, 8):  expression(i, 8) break;                 \
        case __FIELD(u, 32): expression(u, 32) break;                \
        case __FIELD(i, 32): expression(i, 32) break;                \
        case __FIELD(u, 64): expression(u, 64) break;                \
        case __FIELD(i, 64): expression(i, 64) break;                \
    }

/**
 * @brief A macro used to get the enum typename of a signed value.
 */
#define __TYPENAME_i(size) signed##size
/**
 * @brief The sister function of @ref __TYPENAME_i, a macro used to
 * get the enum typename of an unsigned value.
 */
#define __TYPENAME_u(size) unsigned##size
/**
 * @brief A macro to get the field of an ambiguous type.
 */
#define __FIELD(state, size) __TYPENAME_##state(size)

/**
 * @brief Assign a value to an ambiguous type.
 * @param affected The affected variable.
 * @param member What state are we making the ambiguous type?
 * @param value The value that state will have.
 */
void AssignAmbiguousType(AmbiguousType* affected,
                         AmbiguousTypeSpecifier member, void* value);

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
 * @return A boolean value that represents the state of the
 * comparison; true for similarity, false for difference.
 */
__BOOLEAN CompareAmbiguousType(AmbiguousType* affected,
                               AmbiguousTypeSpecifier member,
                               void* value);

#endif // _RENAI_AMBIGUOUS_
