#include "thing.h"


ThingScene::ThingScene() : Scene("light") {
    close = false;
}

void ThingScene::on_create() {
    set_input_window(manager->main_window);
    glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    add_from_file("res/light.scene");

    player = uuid_to_entity(main_camera);
}


void ThingScene::on_update(float dt) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    if(is_key_pressed(GLFW_KEY_ESCAPE))
        close = true;

    actions(this,dt);
    physics(registry,dt);
    if (aabb_check(*this, dt)) return;
    glm::vec2 view = manager->main_window.size();

    opengl_renderer(view, player, registry);
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}


void ThingScene::on_end() {
    EG_TRACE("thing end called");
}

bool ThingScene::should_close() {
    return glfwWindowShouldClose(manager->main_window) || close ;
}
