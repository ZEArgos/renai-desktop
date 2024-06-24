#include "Updater.h"

__CREATE_STRUCT(Updater) CreateUpdater(u8 tick_speed)
{
    Updater* buffer = malloc(sizeof(Updater));
    // Create the cooldown map with a maximum size of 156, since the highest
    // keycode we use is 347, and 347 - 159 - 32 = 156.
    buffer->key_buffer = CreateMap(signed32, signed64, 156);
    buffer->tick_speed = tick_speed;
    return buffer;
}

void KillUpdater(Updater* buffer)
{
    DestroyMap(buffer->key_buffer);
    free(buffer);
}

__BOOLEAN HandleKey(Updater* buffer, i32 key, i32 action)
{
    i32 key_number = (key >= GLFW_KEY_ESCAPE ? key - 159 : key) - 32;
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

void HandleInput(Updater* buffer, Window* key_window, f32 delta_time, i32 key,
                 i32 action)
{
    switch (key)
    {
        case GLFW_KEY_F11:
            if (HandleKey(buffer, GLFW_KEY_F11, action))
                SetWindowFullscreenState(
                    key_window, !glfwGetWindowAttrib(key_window->inner_window,
                                                     GLFW_MAXIMIZED));

            return;
        case GLFW_KEY_F12:
            if (HandleKey(buffer, GLFW_KEY_F12, action))
                glfwSetWindowShouldClose(key_window->inner_window, 1);

            return;
        case GLFW_KEY_BACKSLASH:
            if (HandleKey(buffer, GLFW_KEY_BACKSLASH, action))
            {
                if (glfwGetWindowMonitor(key_window->inner_window) == NULL)
                    SetWindowFullscreenState(key_window, borderless);
                else SetWindowFullscreenState(key_window, maximized);
            }

            return;
        default: break;
    }
}
void UpdateWindowContent(Updater* updater, f32 delta_time) {}
