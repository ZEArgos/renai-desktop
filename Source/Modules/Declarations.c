#include "Declarations.h"
#include "Logger.h"
#include <sys/time.h>
#include <time.h>

void AssignAmbiguousType(AmbiguousType* affected, AmbiguousTypeSpecifier member,
                         void* value)
{
    // Define the body expression of the switch statement. In this case, we
    // simply assign a value to the given member.
#define ASSIGN(state, size)                                                    \
    affected->__FIELD(state, size) = VPTT(state##size, value);
    // Construct the switch statement using our funky little macro.
    __AMBIGUOUS_BODY(member, ASSIGN);
}

__AMBIGUOUS GetAmbiguousType(AmbiguousType* affected,
                             AmbiguousTypeSpecifier member)
{
    // Define the body expression of this function's switch. Here, we simply
    // return the member requested.
#define READ(state, size) return TTVP(affected->__FIELD(state, size));
    // Construct the switch statement using the above body.
    __AMBIGUOUS_BODY(member, READ);
    // This part can't ever happen, but it's here to remove compiler warnings.
    return NULL;
}

__BOOLEAN CompareAmbiguousType(AmbiguousType* affected,
                               AmbiguousTypeSpecifier member, void* value)
{
    // Define the body expression for this particular function's switch
    // statement. Here, we compare the value given and the value of the given
    // member, and return the result.
#define COMPARE(state, size)                                                   \
    if (affected->__FIELD(state, size) == VPTT(state##size, value))            \
        return true;                                                           \
    return false;
    // Construct the switch statement.
    __AMBIGUOUS_BODY(member, COMPARE);
    // This part can never happen, but it's here to assuage compiler concerns.
    return false;
}

__KILLFAIL PollGLFWErrors(const char* caller)
{
    // Get both the error code and human-readable description of the error.
    const char* description;
    i32 code = glfwGetError(&description);

    if (code != 0)
        PrintError("An error happened with GLFW. Code: %d. Message: '%s'.",
                   code, description);
}

__KILLFAIL PollOpenGLErrors(const char* caller)
{
    i32 err = glGetError();
    if (err != 0) PrintError("Ran into an error with OpenGL. Code: %d", err);
}

__PROVIDEDBUFFER GetTimeString(char* buffer, u8 buffer_length)
{
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
    if (s > 60)
    {
        m = s / 60;
        s = s % 60;
    }

    // Set the storage string to a formatted time string, using some string
    // formats so esoteric-looking I'd think Cthulu would be nervous around
    // them.
    snprintf(buffer, buffer_length, "%.*s%hu:%.*s%hhu:%.*s%hu",
             3 - CountDigits(m), "000", (u16)m, 2 - CountDigits(s), "00", (u8)s,
             3 - CountDigits(ms), "000", (u16)ms);
}

__PROVIDEDBUFFER GetDateString(char* buffer, u8 buffer_length)
{
    // Get the raw time, and then the full, local time as reported by the user's
    // computer.
    time_t time_raw = time(NULL);
    struct tm time_full = *localtime(&time_raw);

    snprintf(buffer, buffer_length, "%d-%02d-%02d at %02d:%02d:%02d",
             time_full.tm_year + 1900, time_full.tm_mon + 1, time_full.tm_mday,
             time_full.tm_hour, time_full.tm_min, time_full.tm_sec);
}

/**
 * @brief The exact amount of milliseconds it had been since the beginning of
 * the epoch when the application began.
 */
i64 start_time = 0;

i64 GetCurrentTime(void)
{
    // Use the gettimeofday function to grab the current time information
    // related to this exact moment in time.
    struct timeval time;
    gettimeofday(&time, NULL);

    if (start_time == 0)
    {
        start_time = (i64)time.tv_sec * 1000 + time.tv_usec / 1000;
        return 0;
    }

    // Return the distance between the start time of the application and now.
    return (i64)time.tv_sec * 1000 + time.tv_usec / 1000 - start_time;
}

u16 CountDigits(u32 number)
{
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

/**
 * @brief The total amount of samples allowed in the array before we go back to
 * the beginning.
 */
#define SAMPLE_CAP 25

/**
 * @brief An array of frame lengths, @ref SAMPLE_CAP numbers wide. This is
 * explicitly initialized to 0.
 */
static u32 tick_list[SAMPLE_CAP] = {0};

/**
 * @brief The current sample index we're accessing within the @ref tick_list
 * array.
 */
static u32 current_tick_index = 0;

/**
 * @brief The overall sum of every frame time we've processed so far. This will
 * get more and more accurate as the program runs.
 */
static f64 overall_tick_sum = 0.0;

f64 CalculatePossibleFramerate(u32 new_value)
{
    // Subtract the value from the overall frame sum that we're overriding, and
    // add the new one.
    overall_tick_sum -= tick_list[current_tick_index];
    overall_tick_sum += new_value;
    tick_list[current_tick_index] = new_value;

    // If we've hit the sample cap, send us back to 0.
    current_tick_index = (current_tick_index + 1) % SAMPLE_CAP;

    return overall_tick_sum / SAMPLE_CAP;
}
