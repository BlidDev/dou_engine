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
        player.add_component<TransformComp>((Vector3){ 0.0f, 3.0f, 4.0f }, (Vector3){0.5f, 1.0f, 0.5f});

       player.add_component<Camera>(CameraBuilder().target((Vector3){ 3.0f, 0.0f, -4.0f }).build());
        player.add_component<ActionsComp>(ActionsComp().add(new PlayerAction));
        player.add_component<PhysicsBodyComp>(0.2f, (Vector3){0.0f,0.0f,0.0f},(Vector3){0.0f,0.0f,0.0f}, true, false);
        player.add_component<PrimitiveComp>(PrimitiveComp::Shape::CUBE, BLACK,PRIMITVE_WIREFRAME);

        Entity text = create_entity();
        text.add_component<TextComp>("", 40, BLACK);
        text.add_component<TransformComp>((Vector3){0.0f,2.0f,0.0});
        text.add_component<ActionsComp>(ActionsComp().add(new FPSAction(player.id())));


        Entity plane = create_entity();
        plane.add_component<TransformComp>((Vector3){(float)0.0f,-0.5f,0.0f},(Vector3){(float)1000.0f,0.0f,1000.0f});
        plane.add_component<PrimitiveComp>(PrimitiveComp::Shape::PLANE, DARKGRAY, PRIMITVE_IMMUNE | PRIMITVE_FILLED);
        plane.add_component<PhysicsBodyComp>(0.0f, (Vector3){0.0f,0.0f,0.0f},(Vector3){0.0f,0.0f,0.0f}, true, true);

        for (int i = 0; i < 100; ++i) {
            Entity entity = create_entity();
            entity.add_component<TransformComp>((Vector3){(float)i * 3.0f,(float)i* 0.5f,0.0f});
            entity.add_component<PrimitiveComp>(PrimitiveComp::Shape::CUBE, ColorFromHSV(GetRandomValue(0, 360), 1.0f,1.0f), PRIMITVE_FILLED | PRIMITVE_WIREFRAME);
            auto& ph = entity.add_component<PhysicsBodyComp>(0.0f, (Vector3){0.0f,0.0f,0.0f},(Vector3){0.0f,0.0f,0.0f}, true, true);
            if (i == 99)
            {
                ph.is_solid = false;
                ph.intersects_callback = win_cube;
                entity.add_component<WinCube>(manager,player.id());
            }
            entity.add_component<ActionsComp>(ActionsComp().add(new CubeAction));
        }
    }

    void on_update(float dt) {
        engine::actions(registry, dt);
        engine::physics(registry, dt);
        if(engine::aabb_check(*this, dt)) return;
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


