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

#include <Declarations.h>
#include <Map.h>
#include <Window.h>

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

/**
 * @brief This is a kind of dedicated subfunction for @ref UpdateWindowContent,
 * but for keys pressed by the user. This is a separate function because running
 * it every frame is a waste, instead only calling it via GLFW's @ref
 * keycallback is more efficient.
 * @param buffer The updater to use for this process.
 * @param window The window we will be operating on for keybinds like
 * maximization, etc.
 * @param delta_time The difference in processing time between last frame and
 * this one, to be used in speed normalization.
 * @param key The key pressed.
 */
void HandleInput(Updater* buffer, Window* window, f32 delta_time, i32 key);

/**
 * @brief Similar to the @ref RenderWindowContent function, this updates a
 * window's contents, moving NPC, swapping animation frames, etc.
 * @param updater The updater to use for the process.
 * @param delta_time The difference in processing time between last frame and
 * this one, to be used in speed normalization.
 */
void UpdateWindowContent(Updater* updater, f32 delta_time);

#endif // _RENAI_UPDATER
