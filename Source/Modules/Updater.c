#include "Updater.h"
#include <Logger.h>

/**
 * @brief Transforms a GLFW key code into a compressed key value to be used in
 * the keyboard cooldown buffer.
 * @param key The key to be transformed.
 * @return The key number in the keyboard cooldown map the key corresponds to
 * (0-20).
 */
__INLINE u8 _GetKeyCode(i32 key)
{
    // For the sake of simplicity and conciseness, this is simply an if
    // statement. The values are all self-explanatory.
    return (key >= 290                  ? key - 281 // Function keys (f1 - f12)
            : key >= 262                ? key - 257 // Arrow keys
            : key == GLFW_KEY_ESCAPE    ? 4
            : key == GLFW_KEY_BACKSLASH ? 3
            : key == GLFW_KEY_GRAVE_ACCENT ? 2
            : key == GLFW_KEY_EQUAL        ? 1
            : key == GLFW_KEY_MINUS        ? 0
                                           : 0);
}

/**
 * @brief The delay, in milliseconds, each control key has to endure before
 * their callback can be triggered again.
 */
#define __KEY_DELAY_MS 100

/**
 * @brief Handle the given control key. This function should only be called for
 * the 21 keys that need a cooldown.
 * @param updater The updater to use for the operation.
 * @param key The key pressed.
 * @return A boolean value representing what should be done with the key's
 * callback. True for call, false for wait.
 */
__BOOLEAN _HandleKey(Updater* updater, i32 key)
{
    i64 current_time = GetCurrentTime();
    // Translate the GLFW input key into the one we'll use within the cooldown
    // buffer.
    u8 key_number = _GetKeyCode(key);
    void* stored_value = GetMapItemValue(updater->key_buffer, key_number);

    if (stored_value == NULL)
    {
        AppendMapItem(updater->key_buffer, key_number, current_time);
        return true;
    }

    // If the key's already been hit, check to see if the proper amount of time
    // has past, and reset the time limit and trigger its callback if it has.
    if (current_time - VPTT(i64, stored_value) > __KEY_DELAY_MS)
    {
        EditMapValue(updater->key_buffer, unsigned8, TTVP(key_number), signed64,
                     TTVP(current_time));
        return true;
    }

    return false;
}

__CREATE_STRUCT(Updater) CreateUpdater(u8 tick_speed)
{
    Updater* updater = malloc(sizeof(Updater));
    PrintSuccess("Allocated space for the application's updater: %d bytes.",
                 sizeof(Updater));

    // We only need 21 cooldown characters, each one is a keyboard shortcut of
    // some kind.
    updater->key_buffer = CreateMap(unsigned8, signed64, 21);
    updater->tick_speed = tick_speed;

    PrintSuccess(
        "Created the application's updater successfully. Tick speed: %d o/s",
        tick_speed);
    return updater;
}

void KillUpdater(Updater* updater)
{
    DestroyMap(updater->key_buffer);
    free(updater);
}

void HandleInput(Updater* updater, Window* key_window, f32 delta_time, i32 key)
{
    // We use a switch here both to properly carry out the functionality of
    // control keys and filter out any non-control keys from this first flow.
    switch (key)
    {
        case GLFW_KEY_F11:
            if (_HandleKey(updater, GLFW_KEY_F11))
                ToggleMaximizeWindow(key_window);
            return;
        case GLFW_KEY_F12:
            if (_HandleKey(updater, GLFW_KEY_F12)) CloseWindow(key_window);
            return;
        case GLFW_KEY_BACKSLASH:
            if (_HandleKey(updater, GLFW_KEY_BACKSLASH))
                ToggleFullscreenWindow(key_window);
            return;
        default: break;
    }
}

void UpdateWindowContent(Updater* updater, f32 delta_time)
{
    // There is nothing to update at this point in time, so this function
    // will remain empty for now.
}
