#include "light.h"
#include <engine.h>


LightScene::LightScene() : Scene("light") {
    close = false;
}

void LightScene::on_create() {
    set_input_window(manager->main_window);
    register_shader("res/shaders/basic.glsl");
    manager->register_model("triangle", VAOType::BASIC, engine::P_TRIANGLE, 9);

    Entity tri = create_entity();
    tri.add_component<TransformComp>();
    tri.add_component<ModelComp>(get_model("triangle"), 
                                 MaterialBuilder()
                                     .set_attributes(MODEL_WIREFRAME)
                                     .set_color({1.0f, 0.4f, 0.3f, 1.0f})
                                     .set_shader(get_shader("res/shaders/basic.glsl")));

}
void LightScene::on_update(float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (glfwGetKey(manager->main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        close = true;



    engine::opengl_renderer(registry);
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}

void LightScene::on_end() {
    EG_TRACE("light end called");
}

bool LightScene::should_close() {
    return glfwWindowShouldClose(manager->main_window) || close ;
}
