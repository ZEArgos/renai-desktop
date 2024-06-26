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

#define __KEY_DELAY_MS 100
__BOOLEAN _HandleKey(Updater* updater, u32 key)
{
    i64 current_time = GetCurrentTime();
    u8 key_number = _GetKeyCode(key);
    void* stored_value = GetMapItemValue(updater->key_buffer, key_number);

    if (stored_value == NULL)
    {
        AppendMapItem(updater->key_buffer, key_number, current_time);
        return true;
    }

    if (VPTT(i64, stored_value) > 0 &&
        current_time - VPTT(i64, stored_value) > __KEY_DELAY_MS)
    {
        i64 negated_time = -current_time;
        EditMapValue(updater->key_buffer, unsigned8, TTVP(key_number), signed64,
                     TTVP(negated_time));
        return true;
    }
    else if (VPTT(i64, stored_value) < 0 &&
             VPTT(i64, stored_value) + current_time > __KEY_DELAY_MS)
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
