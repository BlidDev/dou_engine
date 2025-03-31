#pragma once
#include <espch.h>

namespace engine {
    extern GLFWwindow* INPUT_WINDOW;
    extern glm::vec2 MOUSE_LAST_POS;
    void set_input_window(GLFWwindow* window);
    glm::vec2 get_mouse_delta();
    bool is_key_pressed(const int key);
}
