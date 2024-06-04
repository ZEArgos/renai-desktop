/**
 * @file IncludeBoard.h
 * @author Zenais Argos
 * @brief Just a list file of every include expected in each translation unit of
 * the project.
 * @date 2024-06-03
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_INCLUDEBOARD_
#define _RENAI_INCLUDEBOARD_

/**
 * @file stdint.h
 * @brief Provides the various portable types used in the project.
 * @see The C standard library/manual
 */
#include <stdint.h>
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

#endif // _RENAI_INCLUDEBOARD_
