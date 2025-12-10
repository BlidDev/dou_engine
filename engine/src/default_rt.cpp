#include "default_rt.h"
#include "input.h"


namespace engine {


    DefaultRT::DefaultRT() : Scene("Default Runtime") {
    }

    void DefaultRT::on_create() {
        glDisable(GL_DEPTH_TEST);
    }


    void DefaultRT::on_update(float dt) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(manager->main_window);
        glfwPollEvents();
    }


    void DefaultRT::on_end() {
    }

    bool DefaultRT::should_close() {
        return glfwWindowShouldClose(manager->main_window) || is_key_pressed(GLFW_KEY_ESCAPE);
    }
}
