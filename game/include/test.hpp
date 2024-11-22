#pragma once
#include <epch.h>

using namespace engine;

class TestScene : public engine::Scene {
public:
    TestScene() : Scene("test") {
    }

    void on_create() {
        DisableCursor();
        add_from_file("res/test.scene");
    }

    void on_update(float dt) {

        engine::actions(this, dt);
        engine::physics(registry, dt);
        if(engine::aabb_check(*this, dt)) return;
        Entity player = uuid_to_entity(main_camera);
        engine::renderer(player, registry);

        if (IsKeyPressed(KEY_F)){
           manager->set_current("win");
           return;
        }
    }

    void on_end() {
        printf("test end called\n");
    }

    bool should_close() {
        return WindowShouldClose();
    }
};
