#include "Updater.h"
#include <Logger.h>

__CREATE_STRUCT(Updater) CreateUpdater(u8 tick_speed)
{
    Updater* buffer = malloc(sizeof(Updater));
    PrintSuccess("Allocated space for the application's updater: %d bytes.",
                 sizeof(Updater));

    // We only need 21 cooldown characters, each one is a keyboard shortcut of
    // some kind.
    buffer->key_buffer = CreateMap(unsigned8, signed64, 21);
    buffer->tick_speed = tick_speed;

    PrintSuccess(
        "Created the application's updater successfully. Tick speed: %d o/s",
        tick_speed);
    return buffer;
}

void KillUpdater(Updater* buffer)
{
    DestroyMap(buffer->key_buffer);
    free(buffer);
}

__INLINE u8 _GetKeyCode(i32 key)
{
    return (key >= 290                  ? key - 281 // Function keys (f1 - f12)
            : key >= 262                ? key - 257 // Arrow keys
            : key == GLFW_KEY_ESCAPE    ? 4
            : key == GLFW_KEY_BACKSLASH ? 3
            : key == GLFW_KEY_GRAVE_ACCENT ? 2
            : key == GLFW_KEY_EQUAL        ? 1
            : key == GLFW_KEY_MINUS        ? 0
                                           : 0);
}

__BOOLEAN _HandleKey(Updater* buffer, u32 key)
{
    u8 key_number = _GetKeyCode(key);
    i64 current_time = GetCurrentTime();

    void* Updater_value = GetMapItemValue(buffer->key_buffer, key_number);
    if (Updater_value == NULL)
    {
        AppendMapItem(buffer->key_buffer, key_number, current_time);
        return true;
    }

    if (VPTT(i64, Updater_value) - current_time > 0)
        GetMapKeyPair(buffer->key_buffer, key_number)->value.unsigned32 -=
            current_time;
    else RemoveMapItem(buffer->key_buffer, key_number);

    return false;
}

void HandleInput(Updater* buffer, Window* key_window, f32 delta_time, i32 key)
{
    switch (key)
    {
        case GLFW_KEY_F11:
            if (_HandleKey(buffer, GLFW_KEY_F11))
                SetWindowFullscreenState(
                    key_window, !glfwGetWindowAttrib(key_window->inner_window,
                                                     GLFW_MAXIMIZED));

            return;
        case GLFW_KEY_F12:
            if (_HandleKey(buffer, GLFW_KEY_F12))
                glfwSetWindowShouldClose(key_window->inner_window, 1);

            return;
        case GLFW_KEY_BACKSLASH:
            if (_HandleKey(buffer, GLFW_KEY_BACKSLASH))
            {
                if (glfwGetWindowMonitor(key_window->inner_window) == NULL)
                    SetWindowFullscreenState(key_window, borderless);
                else SetWindowFullscreenState(key_window, maximized);
            }

            return;
        default: break;
    }
}

void UpdateWindowContent(Updater* updater, f32 delta_time)
{
    // There is nothing to update at this point in time, so this function
    // will remain empty for now.
}
