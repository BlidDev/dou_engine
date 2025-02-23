#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace engine {
    extern GLFWwindow* INPUT_WINDOW;
    void set_input_window(GLFWwindow* window);
    bool is_key_pressed(const int key);
}
