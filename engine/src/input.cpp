#include "input.h"


namespace engine {

    GLFWwindow* INPUT_WINDOW = nullptr;
    void set_input_window(GLFWwindow* window) {
        INPUT_WINDOW = window;
    }

    bool is_key_pressed(const int key) {
        return glfwGetKey(INPUT_WINDOW,key) == GLFW_PRESS;
    }

}
