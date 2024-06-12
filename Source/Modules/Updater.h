/**
 * @file Updater.h
 * @author Zenais Argos
 * @brief Provides the data structures and functionality needed to update a game
 * window using input from the user.
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_UPDATER_
#define _RENAI_UPDATER_

#include "Declarations.h"

typedef struct KeyBuffer
{
    /**
     * @brief An array of all the keys pressed in the last 50 milliseconds. Note
     * that this is only 256 characters long because Renai only has a handful of
     * keybinds.
     */
    char pressed_keys[256];
    u8 cooldown_left[256];
    i64 cooldown_start[256];
} KeyBuffer;

void HandleInput(KeyBuffer* buffer, GLFWwindow* window);

#endif // _RENAI_UPDATER
