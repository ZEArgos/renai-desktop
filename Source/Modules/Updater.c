#include "Updater.h"

KeyBuffer CreateKeyBuffer(void)
{
    return (struct KeyBuffer){CreateMap(signed32, unsigned32, 301)};
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
}
