// test_scene.cpp

#include "test_scene.h"

TestScene::TestScene() : Scene("Test"){
    close = false;
}

void TestScene::on_create() {
    engine::DU_INFO("Hello!");
    engine::LuaManager::init("res/util.lua");
    s_render_data.clear_color = {0.5f, 0.5f, 0.5f, 1.0f};
    engine::set_layer_depth(manager->render_data, 0, true);

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

    engine::Entity slab = create_entity();
    slab.add_component<engine::TransformComp>(engine::TransformBuilder()
            .position({0.0f, -1.5f, 0.0f})
            .size({6.5f, 0.5f, 6.5f})
            );
    slab.add_component<engine::PhysicsBodyComp>(engine::PhysicsBodyBuilder()
            .gravity(0.0f)
            .is_solid(true)
            .is_static(true)
            );

    slab.add_component<engine::ModelComp>(get_mesh("cube"), engine::MaterialBuilder()
                                                                   .set_color({0.6f, 0.7f, 0.6f})
                                                                   .set_shader(get_shader("basic.glsl"))
                                                                   );
    engine::Entity cube = create_entity();
    cube.add_component<engine::TransformComp>(engine::TransformBuilder()
            .position({-1.0f, 1.0f, 0.0f})
            .size({0.5f, 0.5f, 0.5f})
            );
    cube.add_component<engine::PhysicsBodyComp>(engine::PhysicsBodyBuilder()
            .gravity(5.0f)
            .is_solid(true)
            .is_static(false)
            );

    cube.add_component<engine::ModelComp>(get_mesh("cube"), engine::MaterialBuilder()
                                                                   .set_color({0.4f, 0.7f, 0.9f})
                                                                   .set_shader(get_shader("basic.glsl"))
                                                                   );

    engine::Entity cube2 = create_entity();
    cube2.add_component<engine::TransformComp>(engine::TransformBuilder()
            .position({-1.0f, 0.0f, 3.0f})
            .size({0.35f, 0.35f, 0.35f})
            );

    auto on_enter = [](engine::Scene& a, engine::Entity self, engine::Entity other) {
        if(!other.has_component<engine::TagComp>()) return 0;

        engine::DU_CRITICAL("HOW DARE YOU {}", other.get_component<engine::TagComp>().tag);
        return 0;
    };

    cube2.add_component<engine::LuaActionComp>(cube2.uuid())
        .add(this, "trigger.lua");

    cube2.add_component<engine::PhysicsBodyComp>(engine::PhysicsBodyBuilder()
            .gravity(0.0f)
            .is_solid(false)
            .bind_intersects_callback("trigger.lua", "inter")
            );
    cube2.add_component<engine::ModelComp>(get_mesh("cube"), engine::MaterialBuilder()
                                                                   .set_color({0.2f, 0.2f, 0.2f})
                                                                   .set_shader(get_shader("basic.glsl"))
                                                                   );



    player = create_entity();
    player.add_component<engine::TagComp>("Player");

    player.add_component<engine::TransformComp>(engine::TransformBuilder()
            .position({0.0,0.0, -1.0f})
            .size({0.1f, 1.0f, 0.1f})
            );
    player.add_component<engine::PhysicsBodyComp>(engine::PhysicsBodyBuilder()
            .gravity(9.8f)
            .mass(20.0f)
            .is_solid(true)
            .is_static(false)
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
    engine::fixed_physics(this, 50, dt);

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
 


