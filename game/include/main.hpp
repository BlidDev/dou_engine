#include "engine.h"
#include "actions.h"
#include <raylib.h>
#include <stdio.h>
#include <iostream>


using namespace engine;

class MainScene : public engine::Scene {
public:
    MainScene() : Scene() {
    }

    void on_create() {
        DisableCursor();
        player = create_entity();
        player.add_component<engine::Transform>((Vector3){ 0.0f, 3.0f, 4.0f }, (Vector3){0.5f, 1.0f, 0.5f});

        Camera camera = { 
            .position = (Vector3){ 0.0f, 3.0f, 4.0f },    // Camera positionl
            .target = (Vector3){ 3.0f, 0.0f, -4.0f },      // Camera looking at point
            .up = (Vector3){ 0.0f, 1.0f, 0.0f },          // Camera up vector (rotation towards target)
            .fovy = 90.0f,                                // Camera field-of-view Y
            .projection = CAMERA_PERSPECTIVE
        };

        player.add_component<Camera>(camera);
        player.add_component<Actions>(Actions({new PlayerAction}));
        player.add_component<PhysicsBody>(0.2f, (Vector3){0.0f,0.0f,0.0f},(Vector3){0.0f,0.0f,0.0f}, true, false);
        player.add_component<Primitive>(Primitive::Shape::CUBE, BLACK,PRIMITVE_WIREFRAME);

        Entity text = create_entity();
        text.add_component<Text>("", 40, BLACK);
        text.add_component<engine::Transform>((Vector3){0.0f,2.0f,0.0});
        text.add_component<Actions>(Actions({new FPSAction(player.id())}));


        Entity plane = create_entity();
        plane.add_component<engine::Transform>((Vector3){(float)0.0f,-0.5f,0.0f},(Vector3){(float)1000.0f,0.0f,1000.0f});
        plane.add_component<Primitive>(Primitive::Shape::PLANE, DARKGRAY, PRIMITVE_IMMUNE | PRIMITVE_FILLED);
        plane.add_component<PhysicsBody>(0.0f, (Vector3){0.0f,0.0f,0.0f},(Vector3){0.0f,0.0f,0.0f}, true, true);

        for (int i = 0; i < 100; ++i) {
            Entity entity = create_entity();
            entity.add_component<engine::Transform>((Vector3){(float)i * 3.0f,(float)i* 0.5f,0.0f});
            entity.add_component<Primitive>(Primitive::Shape::CUBE, ColorFromHSV(GetRandomValue(0, 360), 1.0f,1.0f), PRIMITVE_FILLED | PRIMITVE_WIREFRAME);
            auto& ph = entity.add_component<PhysicsBody>(0.0f, (Vector3){0.0f,0.0f,0.0f},(Vector3){0.0f,0.0f,0.0f}, true, true);
            if (i == 99)
            {
                ph.is_solid = false;
                ph.intersects_callback = win_cube;
                entity.add_component<WinCube>(manager,player.id());
            }
            entity.add_component<Actions>(Actions({new CubeAction}));
        }
    }

    void on_update(float dt) {
        engine::actions(registry, dt);
        engine::physics(registry, dt);
        if(engine::aabb_check(registry, dt)) return;
        engine::renderer(player, registry);
    }

    void on_end() {
        printf("main end called\n");
    }

    bool should_close() {
        return WindowShouldClose();
    }
private:
    engine::Entity player;
};


