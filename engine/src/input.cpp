#include "input.h"


namespace engine {

    GLFWwindow* INPUT_WINDOW = nullptr;
    glm::vec2 MOUSE_LAST_POS = {0.0f,0.0f};
    std::array<int, GLFW_KEY_LAST - 32> key_query;
    std::array<int, GLFW_KEY_LAST - 32> old_key_query;

    void set_input_window(GLFWwindow* window) {
        INPUT_WINDOW = window;

        for (int i = 0; i < key_query.size(); i++) {
            key_query[i] = GLFW_RELEASE;
            old_key_query[i] = GLFW_RELEASE;
        }

        glfwSetKeyCallback(window, default_key_callback);
    }

    void default_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        size_t index = key - 32;
        old_key_query[index] = key_query[index];
        key_query[index] = action;

    }

    bool is_key_pressed(const int key) {
        DU_ASSERT(!INPUT_WINDOW, "Input window is not set or destroyed");
        size_t index = key - 32;
        bool pressed = (key_query[index] == GLFW_PRESS) || (key_query[index] == GLFW_REPEAT); 
        if(pressed)
            old_key_query[index] = GLFW_PRESS;
        return pressed;
    }

    bool is_key_released(const int key) {
        DU_ASSERT(!INPUT_WINDOW, "Input window is not set or destroyed");
        size_t index = key - 32;
        bool old_pressed = (old_key_query[index] == GLFW_PRESS) || (old_key_query[index] == GLFW_REPEAT);

        if (key_query[index] == GLFW_RELEASE && old_pressed) {
            old_key_query[index] = GLFW_RELEASE;
            return true;
        }
        return false;
    }

    bool is_key_clicked(const int key) {
        DU_ASSERT(!INPUT_WINDOW, "Input window is not set or destroyed");
        size_t index = key - 32;
        bool old_released = old_key_query[index] == GLFW_RELEASE;

        if (key_query[index] == GLFW_PRESS && old_released) {
            old_key_query[index] = GLFW_PRESS;
            return true;
        }
        return false;
    }

    glm::vec2 get_mouse_delta() {
        double x,y;
        glfwGetCursorPos(INPUT_WINDOW, &x,&y);
        glm::vec2 pos = {(float)x, (float)y};
        glm::vec2 delta = MOUSE_LAST_POS - pos;
        MOUSE_LAST_POS = pos;
        return delta;
    }

    bool is_mouse_pressed(const int button) {
        DU_ASSERT(!INPUT_WINDOW, "Input window is not set or destroyed");
        return glfwGetMouseButton(INPUT_WINDOW, button) == GLFW_PRESS;
    }

    bool is_mouse_released(const int button) {
        DU_ASSERT(!INPUT_WINDOW, "Input window is not set or destroyed");
        return glfwGetMouseButton(INPUT_WINDOW, button) == GLFW_RELEASE;
    }

    void set_input_mode(Scene* scene, int value, int mode) {
        glfwSetInputMode(scene->manager->main_window, value, mode);
    }

}
