#include "light.h"
#include <engine.h>
#include "actions.h"

LightScene::LightScene() : Scene("light") {
    close = false;
}

void LightScene::on_create() {
    set_input_window(manager->main_window);
    register_shader("res/shaders/basic.glsl");
    manager->register_model("triangle", VAOType::BASIC, engine::P_TRIANGLE, 9);
    manager->register_model("cube", VAOType::BASIC, engine::P_CUBE, 54);

    int a =5;
    player = create_entity();
    auto& t = player.add_component<TransformComp>(engine::TransformBuilder()
            .position({0.0f, 0.0f ,-2.0f}));
    auto& c = player.add_component<CameraComp>(CameraBuilder().target({0.0,0.0,0.0}).build());
    c.last_pos = t.position;
    player.add_component<PhysicsBodyComp>(PhysicsBodyBuilder().is_solid(false).is_static(true).gravity(0.0f));
    player.add_component<ActionsComp>(engine::ActionsComp({new SimpleAct}));
    
    Entity tri = create_entity();
    tri.add_component<TransformComp>(engine::TransformBuilder()
                                    .position({0.0f, 0.0f ,0.0f}));
    tri.add_component<ModelComp>(get_model("cube"), 
                                 MaterialBuilder()
                                     .set_color({1.0f, 0.4f, 0.3f, 1.0f})
                                     .set_shader(get_shader("res/shaders/basic.glsl")));

}
void LightScene::on_update(float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (glfwGetKey(manager->main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        close = true;



    actions(this,dt);
    physics(registry,dt);
    glm::vec2 view = manager->main_window.size();
    opengl_renderer(view,player, registry);
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}

void LightScene::on_end() {
    EG_TRACE("light end called");
}

bool LightScene::should_close() {
    return glfwWindowShouldClose(manager->main_window) || close ;
}
