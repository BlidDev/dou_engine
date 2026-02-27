#include "thing.h"
#include "renderer.h"

using namespace engine;

ThingScene::ThingScene() : Scene("thing") {
    close = false;
}

void ThingScene::on_create() {

    //glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    add_from_file("res/thing.scene");

    player = uuid_to_entity(main_camera);

    Entity push = uuid_to_entity(2240326152093191729);
    auto& trans = push.get_component<TransformComp>();
    auto& model =  push.get_component<ModelComp>();
    auto& phy =  push.get_component<PhysicsBodyComp>();

    for (int x = 1; x < 4; x++) {
        for (int y = 1; y < 4; y++) { 
            Entity tmp = create_entity();
            auto& t = tmp.add_component<TransformComp>(trans);
            t.translate({x * t.size().x * 4.3f, 0.0f, y * t.size().z * 1.1});
            tmp.add_component<ModelComp>(model);
            tmp.add_component<PhysicsBodyComp>(phy);
        }
    }
    lua_action_init(this);
    actions_init(this);
}


void ThingScene::on_update(float dt) {
    close = engine::is_key_pressed(GLFW_KEY_ESCAPE);


    engine::lua_action_update(this, dt);
    engine::fixed_physics(this, 50, dt);

    glm::vec2 view_size = manager->main_window.size();

    engine::rescale_camera_to_window(player.get_component<engine::CameraComp>(), manager->main_window);
    engine::draw_to_camera(manager->render_data, view_size, player, registry, &s_render_data);
    engine::present_camera(player, get_mesh("DefaultDisplayQuad"));

    manager->main_window.swap_and_poll();
}


void ThingScene::on_end() {
    DU_TRACE("thing end called");
    actions_end(this);
    lua_action_end(this);
}

bool ThingScene::should_close() {
    return glfwWindowShouldClose(manager->main_window) || close ;
}
