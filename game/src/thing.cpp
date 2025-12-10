#include "thing.h"
#include "renderer.h"

using namespace engine;

ThingScene::ThingScene() : Scene("thing") {
    close = false;
}

void ThingScene::on_create() {

    glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    add_from_file("res/thing.scene");

    player = uuid_to_entity(main_camera);
    lua_action_init(this);
}


void ThingScene::on_update(float dt) {
    close = is_key_pressed(GLFW_KEY_ESCAPE);
    actions(this,dt);
    lua_action_update(this, dt);
    physics(registry,dt);
    if (aabb_check(*this, dt)) return;
    glm::vec2 view = manager->main_window.size();

    rescale_camera_to_window(player.get_component<CameraComp>(), manager->main_window);
    draw_to_camera(manager->render_data, view, player, registry, &s_render_data);
    present_camera(player, get_model("quad_tex"));
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();

}


void ThingScene::on_end() {
    DU_TRACE("thing end called");
    lua_action_end(this);
}

bool ThingScene::should_close() {
    return glfwWindowShouldClose(manager->main_window) || close ;
}
