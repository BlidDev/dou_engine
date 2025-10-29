#pragma once
#include <espch.h>
#include "manager.h"

namespace engine {
    extern GLFWwindow* INPUT_WINDOW;
    extern glm::vec2 MOUSE_LAST_POS;
    void set_input_window(GLFWwindow* window);
    glm::vec2 get_mouse_delta();
    bool is_key_pressed(const int key);
    bool is_mouse_pressed(const int button);

    void set_input_mode(Scene* scene, int value, int mode);
}
