#include "Updater.h"

KeyBuffer* CreateKeyBuffer(void)
{
    KeyBuffer* buffer = malloc(sizeof(KeyBuffer));
    // Create the cooldown map with a maximum size of 156, since the highest
    // keycode we use is 347, and 347 - 159 - 32 = 156.
    buffer->cooldown_map = CreateMap(signed32, signed64, 156);
    return buffer;
}

void KillKeyBuffer(KeyBuffer* buffer)
{
    DestroyMap(buffer->cooldown_map);
    free(buffer);
}

__BOOLEAN HandleKey(KeyBuffer* buffer, i32 key, i32 action)
{
    i32 key_number = (key >= GLFW_KEY_ESCAPE ? key - 159 : key) - 32;
    void* keybuffer_value = GetMapItemValue(buffer->cooldown_map, key_number);

    i64 current_time = GetCurrentTime();
    if (keybuffer_value == NULL)
    {
        AppendMapItem(buffer->cooldown_map, key_number, current_time);
        return true;
    }
    else if ((i32)VPTT(u32, keybuffer_value) - current_time > 0)
        GetMapKeyPair(buffer->cooldown_map, key_number)->value.unsigned32 -=
            current_time;
    else RemoveMapItem(buffer->cooldown_map, key_number);

    return false;
}

void HandleInput(KeyBuffer* buffer, Window* key_window, f32 delta_time, i32 key,
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
