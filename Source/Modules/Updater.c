#include "Updater.h"

KeyBuffer* CreateKeyBuffer(void)
{
    KeyBuffer* buffer = malloc(sizeof(KeyBuffer));
    buffer->cooldown_map = CreateMap(signed32, unsigned32, 179);
    return buffer;
}

void KillKeyBuffer(KeyBuffer* buffer)
{
    DestroyMap(buffer->cooldown_map);
    free(buffer);
}

#define CastVoidPointerToType(type, pointer) *((type*)pointer)
u8 HandleKey(KeyBuffer* buffer, i32 key, i32 action, u32 time_since_last)
{
    void* keybuffer_value = GetMapItemValue(buffer->cooldown_map, key - 32);

    if (keybuffer_value == NULL)
    {
        AppendMapItem(buffer->cooldown_map, key - 32, 25);
        return SUCCESS;
    }
    else if ((i32)CastVoidPointerToType(u32, keybuffer_value) -
                 (i32)time_since_last >
             0)
        GetMapKeyPair(buffer->cooldown_map, key - 32)->value.unsigned32 -=
            time_since_last;
    else RemoveMapItem(buffer->cooldown_map, key - 32);

    return FAILURE;
}

void HandleInput(KeyBuffer* buffer, Window* key_window, i32 key, i32 action,
                 u32 time_since_last)
{
    switch (key)
    {
        case GLFW_KEY_F11:
            if (HandleKey(buffer, GLFW_KEY_F11 - 123, action, time_since_last))
                SetWindowFullscreenState(
                    key_window, !glfwGetWindowAttrib(key_window->inner_window,
                                                     GLFW_MAXIMIZED));

            return;
        case GLFW_KEY_F12:
            if (HandleKey(buffer, GLFW_KEY_F12 - 123, action, time_since_last))
                glfwSetWindowShouldClose(key_window->inner_window, 1);

            return;
        case GLFW_KEY_BACKSLASH:
            if (HandleKey(buffer, GLFW_KEY_BACKSLASH, action, time_since_last))
            {
                if (glfwGetWindowMonitor(key_window->inner_window) == NULL)
                    SetWindowFullscreenState(key_window, borderless);
                else SetWindowFullscreenState(key_window, maximized);
            }

            return;
        default: break;
    }
}
