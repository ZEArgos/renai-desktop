/**
 * @file Updater.h
 * @author Zenais Argos
 * @brief Provides the data structures and functionality needed to
 * update a game window using input from the user.
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_UPDATER_
#define _RENAI_UPDATER_

// This is used for its type definitions and utility macros in this
// file.
#include <Declarations.h>
// This is needed to properly store key-value pairs in the renderer's
// keybuffer.
#include <Map.h>
// We use helper functions from this file to handle window-related
// control shortcuts.
#include <Window.h>

/**
 * @brief A structure to hold the data relevant to updating a window
 * each frame. Well, that description isn't entirely perfect, as the
 * actual application data (NPCs, sprites, player characteristics,
 * etc.) are all stored in a different object. This simply @b
 * facilitates that work.
 */
typedef struct Updater
{
    /**
     * @brief This value is basically the game's internal measurement
     * of how fast things should be moving in-game. This represents
     * how many "turns" each object can take each second; the higher
     * the value, the more an NPC is allowed to move, or sell items,
     * or etc.
     */
    u8 tick_speed;
    /**
     * @brief A buffer to hold each key that has been pressed in the
     * last 100 milliseconds, each awaiting their turn to be pressed
     * again. There are only 21 items in this map, because only 21
     * keys do a function in-game.
     */
    Map* key_buffer;
} Updater;

/**
 * @brief Create an updater object.
 * @param tick_speed The updater's default tickspeed.
 * @return A pointer to the object just created.
 */
__CREATE_STRUCT(Updater)
CreateUpdater(u8 tick_speed);

/**
 * @brief Kill the updater. This frees all resources related to the
 * object.
 * @param updater The updater to kill.
 */
__INLINE void KillUpdater(Updater* updater)
{
    DestroyMap(updater->key_buffer);
    __FREE(updater,
           ("The updater freer was given an invalid texture."));
}

/**
 * @brief This is a kind of dedicated subfunction for @ref
 * UpdateWindowContent, but for keys pressed by the user. This is a
 * separate function because running it every frame is a waste,
 * instead only calling it via GLFW's @ref keycallback is more
 * efficient.
 * @param updater The updater to use for this process.
 * @param window The window we will be operating on for keybinds like
 * maximization, etc.
 * @param delta_time The difference in processing time between last
 * frame and this one, to be used in speed normalization.
 * @param key The key pressed.
 */
void HandleInput(Updater* updater, Window* window, f32 delta_time,
                 i32 key);

/**
 * @brief Similar to the @ref RenderWindowContent function, this
 * updates a window's contents, moving NPC, swapping animation frames,
 * etc.
 * @param updater The updater to use for the process.
 * @param delta_time The difference in processing time between last
 * frame and this one, to be used in speed normalization.
 */
void UpdateWindowContent(Updater* updater, f32 delta_time);

#endif // _RENAI_UPDATER
