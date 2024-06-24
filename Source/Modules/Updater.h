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
#include "Window.h"
#include <Map.h>

typedef struct KeyBuffer
{
    Map* cooldown_map;
} KeyBuffer;

KeyBuffer* CreateKeyBuffer(void);
void KillKeyBuffer(KeyBuffer* buffer);

void HandleInput(KeyBuffer* buffer, Window* window, f32 delta_time, i32 key,
                 i32 action);

#endif // _RENAI_UPDATER
