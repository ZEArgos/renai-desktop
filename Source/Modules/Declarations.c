#include "Declarations.h" // The Declarations mother header. This provides all the function declarations.
#include "Logger.h"   // The logger header, for use in logging output codes.
#include <sys/time.h> // The system header for geting the current time and date.
#include <time.h> // The C standard header for getting time and date formatting correct.

__KILLFAIL PrintGLFWError(const char* caller)
{
    // Get both the error code and human-readable description of the error.
    const char* description;
    i32 code = glfwGetError(&description);

    // If the code was not nothing, print the error and return false.
    if (code != 0)
        // Try to print the message to console.
        PrintError("An error happened with GLFW. Code: %d. Message: '%s'.",
                   code, description);
}

__BOOLEAN PrintGLError(const char* caller)
{
    // Try to get the error code, and if we find one, print it and return
    // failure.
    i32 err = glGetError();
    if (err != 0)
    {
        PrintError("Ran into an error with OpenGL. Code: %d", err);
        return false;
    }
    // Return that nothing has yet gone amiss.
    return true;
}

void GetDateString(char* buffer)
{
    // Get the raw time, and then the full, local time as reported by the user's
    // computer.
    time_t time_raw = time(NULL);
    struct tm time_full = *localtime(&time_raw);

    // Print the raw time into the buffer, with a maxlength of 64 characters.
    // Note that we do some seemingly-strange addition with some of the
    // variables, but that's simply because we're reformatting some date values.
    snprintf(buffer, 64, "%d-%02d-%02d at %02d:%02d:%02d",
             time_full.tm_year + 1900, time_full.tm_mon + 1, time_full.tm_mday,
             time_full.tm_hour, time_full.tm_min, time_full.tm_sec);
}

u16 CountDigits(u32 number)
{
    // Allocate some space in which we will store the number in string form.
    char string[255];
    // Try to print the number into the string. If this, for some reason, fails,
    // print a warning in the terminal and return false.
    if (snprintf(string, 255, "%d", number) < 0)
    {
        PrintWarning("Failed to get the number of digits in number %d.",
                     number);
        return false;
    }
    // Return the length of the number-string, cast to an unsigned, 16-bit
    // integer as the numbers we deal with will not be longer.
    return (u16)strlen(string);
}

// Initialize the application's start time to zero, to prevent any garbage data
// being parsed as an actual time.
i64 start_time = 0;
i64 GetCurrentTime(void)
{
    // Use the gettimeofday function to grab the current time information
    // related to this exact moment in time, and store it in "time".
    struct timeval time;
    gettimeofday(&time, NULL);

    // If the application's start time has not yet been defined, do it.
    if (start_time == 0)
    {
        // Do some esoteric calculations to convert the number to milliseconds,
        // and return zero from the function.
        start_time = (i64)time.tv_sec * 1000 + time.tv_usec / 1000;
        return 0;
    }
    // Return the distance between the start time of the application and this
    // moment in time.
    return (i64)time.tv_sec * 1000 + time.tv_usec / 1000 - start_time;
}

void GetTimeString(char* storage, u32 string_size)
{
    // Get the distance between the start of the application's runtime and right
    // now.
    i64 ms = GetCurrentTime(), s = 0, m = 0;
    // If the count of milliseconds is over 1000 (aka 1 second), convert it
    // over.
    if (ms > 1000)
    {
        s = ms / 1000;
        // Use the modulus operator, as we need the remainder after the
        // division.
        ms = ms % 1000;
    }
    // If the seconds is over 60 (aka 1 minute), convert it over, using the same
    // operation(s) as for the milliseconds.
    if (s > 60)
    {
        m = s / 60;
        s = s % 60;
    }

    // Set the storage string to a formatted time string, using some string
    // formats so esoteric-lookin' I'd think Cthulu would be nervous around
    // them.
    snprintf(storage, string_size, "%.*s%ldms %.*s%lds %.*s%ldm",
             3 - CountDigits(ms), "000", ms, 2 - CountDigits(s), "00", s,
             3 - CountDigits(m), "000", m);
}

#define TYPENAME_i(size)   CONCAT(signed, size)
#define TYPENAME_u(size)   CONCAT(unsigned, size)
#define FIELD(state, size) CONCAT(TYPENAME_, state)(size)

#define ASSIGN(state, size)                                                    \
    affected->FIELD(state, size) = VPTT(__CONCAT(state, size), value);
#define READ(state, size) return TTVP(affected->FIELD(state, size));
#define COMPARE(state, size)                                                   \
    if (affected->FIELD(state, size) == VPTT(__CONCAT(state, size), value))    \
        return true;                                                           \
    return false;

#define __AMBIGUOUS_BODY(TYPE, BODY)                                           \
    switch (TYPE)                                                              \
    {                                                                          \
        case FIELD(u, 32): BODY(u, 32) break;                                  \
        case FIELD(i, 32): BODY(i, 32) break;                                  \
        case FIELD(u, 64): BODY(u, 64) break;                                  \
        case FIELD(i, 64): BODY(i, 64) break;                                  \
        default:           break;                                                        \
    }
#define FIELD(state, size) CONCAT(TYPENAME_, state)(size)

void AssignAmbiguousType(AmbiguousType* affected, AmbiguousTypeSpecifier member,
                         void* value)
{
    __AMBIGUOUS_BODY(member, ASSIGN);
}

void* GetAmbiguousType(AmbiguousType* affected, AmbiguousTypeSpecifier member)
{
    __AMBIGUOUS_BODY(member, READ);
    return NULL;
}

__BOOLEAN CompareAmbiguousType(AmbiguousType* affected,
                               AmbiguousTypeSpecifier member, void* value)
{
    __AMBIGUOUS_BODY(member, COMPARE);
    return false;
}
