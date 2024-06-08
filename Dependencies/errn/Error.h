/**
 * @file Error.h
 * @author Zenais Argos
 * @brief Thos
 * @date 2024-06-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <inttypes.h>

/**
 * @brief Defines the severity of a given error.
 */
typedef enum ErrorSeverity
{
    /**
     * @brief This error is little more than a warning. The application can
     * continue on after its calling, with little to no problems. These kind of
     * errors will not be made available to the user unless they check the debug
     * log file(s).
     */
    low,
    /**
     * @brief The error is severe, but the application can attempt survival with
     * reasonable confidence in its own success. These errors will be made known
     * to the user as they happen, but the application will continue on.
     */
    severe,
    /**
     * @brief The error poses an immediately risk to the process or user's
     * security/function. The process will shut down immediately.
     */
    fatal
} ErrorSeverity;

/**
 * @brief A structure to contain all of the information about a thrown error.
 * This will be constructed by the @ref ThrowError function, please do not
 * construct one of these as a user.
 */
typedef struct Error
{
    /**
     * @brief The given error code. These are defined with verbose names below.
     */
    uint32_t code;
    /**
     * @brief The severity of the error. This will be assigned by @ref
     * ThrowError based on the error code.
     */
    ErrorSeverity severity;
    /**
     * @brief The name of the function which called @ref ThrowError.
     */
    char* caller_name;
    /**
     * @brief The message to be displayed along with the error.
     */
    char* message;
} Error;

#define ThrowError(code, message) __ThrowError(code, __FUNCTION__, message)
void __ThrowError(uint32_t code, char* name, char* message);
void HandleError(Error* err);
