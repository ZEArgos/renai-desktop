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

typedef struct Updater
{
    /**
     * @brief This value is basically the game's internal measurement of how
     * fast things should be moving in-game. This represents how many "turns"
     * each object can take each second; the higher the value, the more an NPC
     * is allowed to move, or sell items, or etc.
     */
    u8 tick_speed;
    Map* key_buffer;
} Updater;

__CREATE_STRUCT(Updater) CreateUpdater(u8 tick_speed);
void KillUpdater(Updater* buffer);

void HandleInput(Updater* buffer, Window* window, f32 delta_time, i32 key,
                 i32 action);

/**
 * @brief Similar to the @ref RenderWindowContent function, this updates a
 * window's contents, moving NPC, swapping animation frames, etc.
 * @param updater The updater to use for the process.
 */
void UpdateWindowContent(Updater* updater, f32 delta_time);

#endif // _RENAI_UPDATER
