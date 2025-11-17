#include "input.h"


namespace engine {

    GLFWwindow* INPUT_WINDOW = nullptr;
    glm::vec2 MOUSE_LAST_POS = {0.0f,0.0f};
    void set_input_window(GLFWwindow* window) {
        INPUT_WINDOW = window;
    }

    glm::vec2 get_mouse_delta() {
        double x,y;
        glfwGetCursorPos(INPUT_WINDOW, &x,&y);
        glm::vec2 pos = {(float)x, (float)y};
        glm::vec2 delta = MOUSE_LAST_POS - pos;
        MOUSE_LAST_POS = pos;
        return delta;
    }

    bool is_key_pressed(const int key) {
        DU_ASSERT(!INPUT_WINDOW, "Input window is not set or destroyed");
        return glfwGetKey(INPUT_WINDOW, key) == GLFW_PRESS;
    }

    bool is_mouse_pressed(const int button) {
        DU_ASSERT(!INPUT_WINDOW, "Input window is not set or destroyed");
        return glfwGetMouseButton(INPUT_WINDOW, button) == GLFW_PRESS;
    }

    void set_input_mode(Scene* scene, int value, int mode) {
        glfwSetInputMode(scene->manager->main_window, value, mode);
    }

}
