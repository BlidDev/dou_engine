#include "light.h"


LightScene::LightScene() : Scene("light") {
    close = false;
}

void LightScene::on_create() {
}

void LightScene::on_update(float dt) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if (glfwGetKey(manager->main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        close = true;
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}

void LightScene::on_end() {
    EG_TRACE("light end called");
}

bool LightScene::should_close() {
    return glfwWindowShouldClose(manager->main_window) || close ;
}
