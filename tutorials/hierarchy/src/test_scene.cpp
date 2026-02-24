// test_scene.cpp

#include "test_scene.h"

TestScene::TestScene() : Scene("Test"){
    close = false;
}

void TestScene::on_create() {
    engine::DU_INFO("Hello!");

    add_from_file("res/test_scene.scene");

    player = uuid_to_entity(main_camera); // remember we need to convert it from UUID to an actual Entity
    
    engine::lua_action_init(this);
}

void TestScene::on_update(float dt) {
    close = engine::is_key_pressed(GLFW_KEY_ESCAPE);


    engine::lua_action_update(this, dt);
    engine::fixed_physics(this, 50, dt);

    glm::vec2 view_size = manager->main_window.size();

    engine::rescale_camera_to_window(player.get_component<engine::CameraComp>(), manager->main_window);
    engine::draw_to_camera(manager->render_data, view_size, player, registry, &s_render_data);
    engine::present_camera(player, get_mesh("DefaultDisplayQuad"));

    manager->main_window.swap_and_poll();
}

void TestScene::on_end() {
    engine::lua_action_end(this);
    engine::DU_INFO("Bye!");
}

bool TestScene::should_close() {
    return close || glfwWindowShouldClose(manager->main_window);
}
 


