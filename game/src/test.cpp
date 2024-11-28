#include "test.hpp"


TestScene::TestScene() : Scene("test") {
}

void TestScene::on_create() {
    DisableCursor();
    add_from_file("res/test.scene");
    lua_action_init(this);
}

void TestScene::on_update(float dt) {

    engine::actions(this, dt);
    engine::lua_action_update(this, dt);
    engine::physics(registry, dt);
    if(engine::aabb_check(*this, dt)) return;
    Entity player = uuid_to_entity(main_camera);
    engine::renderer(player, registry);
}

void TestScene::on_end() {
    EG_TRACE("test end called");
    lua_action_end(this);
}

bool TestScene::should_close() {
    return WindowShouldClose();
}
