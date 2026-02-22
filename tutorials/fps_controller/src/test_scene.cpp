// test_scene.cpp

#include "test_scene.h"

TestScene::TestScene() : Scene("Test"){
    close = false;
}

void TestScene::on_create() {
    engine::DU_INFO("Hello!");
    s_render_data.clear_color = {0.5f, 0.5f, 0.5f, 1.0f};

    engine::Entity triangle = create_entity();

    // we could also leave the TransformComp constructor empty but we want to change some default stuff
    triangle.add_component<engine::TransformComp>(engine::TransformBuilder()
            .position(glm::vec3(0.0f))
            .size({1.0f, 1.5f, 1.0f})
            );

    triangle.add_component<engine::ModelComp>(get_mesh("triangle"), engine::MaterialBuilder()
                                                                   .set_color({1.0f, 0.0f, 1.0f})
                                                                   .set_shader(get_shader("basic.glsl"))
                                                                   );
    player = create_entity();
    player.add_component<engine::TransformComp>(engine::TransformBuilder()
            .position({0.0,0.0, -1.0f})
            );

    auto size = manager->main_window.size();
    player.add_component<engine::CameraComp>(engine::CameraBuilder()
             .projection_mode(engine::CameraProjection::Perspective)
             .pitch(0.0f)
             .yaw(90.0f)
             .fovy(90.0f)
             .framebuffer_size(size.x, size.y)
             .present_shader(get_shader("camera_present.glsl"))
             .build()
             );

    player.add_component<engine::LuaActionComp>(player.uuid())
        .add(this, "fps_controller.lua");


    engine::lua_action_init(this);
}

void TestScene::on_update(float dt) {
    close = engine::is_key_pressed(GLFW_KEY_ESCAPE);

    engine::lua_action_update(this, dt);

    glm::vec2 view_size = manager->main_window.size();

    engine::rescale_camera_to_window(player.get_component<engine::CameraComp>(), manager->main_window);
    engine::draw_to_camera(manager->render_data, view_size, player, registry, &s_render_data);
    engine::present_camera(player, get_mesh("quad_tex"));

    manager->main_window.swap_and_poll();}

void TestScene::on_end() {
    engine::lua_action_end(this);
    engine::DU_INFO("Bye!");
}

bool TestScene::should_close() {
    return close || glfwWindowShouldClose(manager->main_window);
}
 


