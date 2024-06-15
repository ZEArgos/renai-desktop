#include "Updater.h"

KeyBuffer* CreateKeyBuffer(void)
{
    KeyBuffer* buffer = malloc(sizeof(KeyBuffer));
    buffer->cooldown_map = CreateMap(signed32, unsigned32, 301);
    return buffer;
}

void KillKeyBuffer(KeyBuffer* buffer)
{
    DestroyMap(buffer->cooldown_map);
    free(buffer);
}

#define CastVoidPointerToType(type, pointer) *((type*)pointer)
void HandleInput(KeyBuffer* buffer, Window* key_window)
{
    void* f11_keybuffer_value =
        GetMapItemValue(buffer->cooldown_map, GLFW_KEY_F11);
    if (glfwGetKey(key_window->inner_window, GLFW_KEY_F11) == GLFW_PRESS &&
        f11_keybuffer_value == NULL)
    {
        SetWindowFullscreenState(
            key_window,
            !glfwGetWindowAttrib(key_window->inner_window, GLFW_MAXIMIZED));

        AppendMapItem(buffer->cooldown_map, GLFW_KEY_F11, 50);
    }
    else if (f11_keybuffer_value != NULL)
    {
        if (CastVoidPointerToType(u32, f11_keybuffer_value) > 0)
            GetMapKeyPair(buffer->cooldown_map, GLFW_KEY_F11)
                ->value.unsigned32 -= 1;
        else RemoveMapItem(buffer->cooldown_map, GLFW_KEY_F11);
    }

    void* f12_keybuffer_value =
        GetMapItemValue(buffer->cooldown_map, GLFW_KEY_F12);
    if (glfwGetKey(key_window->inner_window, GLFW_KEY_F12) == GLFW_PRESS &&
        f12_keybuffer_value == NULL)
    {
        glfwSetWindowShouldClose(key_window->inner_window, 1);

        AppendMapItem(buffer->cooldown_map, GLFW_KEY_F12, 50);
    }
    else if (f12_keybuffer_value != NULL)
    {
        if (CastVoidPointerToType(u32, f12_keybuffer_value) > 0)
            GetMapKeyPair(buffer->cooldown_map, GLFW_KEY_F12)
                ->value.unsigned32 -= 1;
        else RemoveMapItem(buffer->cooldown_map, GLFW_KEY_F12);
    }

    void* backslash_keybuffer_value =
        GetMapItemValue(buffer->cooldown_map, GLFW_KEY_BACKSLASH);
    if (glfwGetKey(key_window->inner_window, GLFW_KEY_BACKSLASH) ==
            GLFW_PRESS &&
        backslash_keybuffer_value == NULL)
    {
        if (glfwGetWindowMonitor(key_window->inner_window) == NULL)
            SetWindowFullscreenState(key_window, borderless);
        else SetWindowFullscreenState(key_window, maximized);

        AppendMapItem(buffer->cooldown_map, GLFW_KEY_BACKSLASH, 50);
    }
    else if (backslash_keybuffer_value != NULL)
    {
        if (CastVoidPointerToType(u32, backslash_keybuffer_value) > 0)
            GetMapKeyPair(buffer->cooldown_map, GLFW_KEY_BACKSLASH)
                ->value.unsigned32 -= 1;
        else RemoveMapItem(buffer->cooldown_map, GLFW_KEY_BACKSLASH);
    }
}
