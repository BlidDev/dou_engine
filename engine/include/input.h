#pragma once
#include <espch.h>
#include "manager.h"

namespace engine {
    extern GLFWwindow* INPUT_WINDOW;
    extern glm::vec2 MOUSE_LAST_POS;

    extern std::array<int, GLFW_KEY_LAST - 32> key_query;
    extern std::array<int, key_query.size()> old_key_query;

    void set_input_window(GLFWwindow* window);
    void default_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    glm::vec2 get_mouse_delta();
    bool is_key_pressed(const int key);
    bool is_key_released(const int key);
    bool is_key_clicked(const int key);
    bool is_mouse_pressed(const int button);
    bool is_mouse_released(const int button);

    void set_input_mode(Scene* scene, int value, int mode);
}
