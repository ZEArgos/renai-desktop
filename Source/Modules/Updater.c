#include "Updater.h"

void HandleInput(KeyBuffer* buffer, GLFWwindow* window)
{
    if (strpbrk(buffer->pressed_keys, "0") == 0 &&
        glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        buffer->cooldown_left[strlen(buffer->pressed_keys)] = 50;
        buffer->cooldown_start[strlen(buffer->pressed_keys)] = GetCurrentTime();
        buffer->pressed_keys[strlen(buffer->pressed_keys)] = '0';

        printf("e\n");
    }
}
