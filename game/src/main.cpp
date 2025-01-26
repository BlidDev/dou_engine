#include "main.hpp"
#include "actions.h"
#include <algorithm>

using namespace engine;


MainScene::MainScene() : Scene("main") {
}

void MainScene::on_create() {
    DisableCursor();
    player = create_entity();
    player.add_component<TransformComp>((Vector3){ 0.0f, 3.0f, 4.0f }, (Vector3){0.5f, 1.0f, 0.5f});

    player.add_component<TagComp>("Player");
    player.add_component<Camera>(CameraBuilder().target((Vector3){ 3.0f, 0.0f, -4.0f }).build());
    player.add_component<ActionsComp>(ActionsComp().add("PlayerAction"));
    player.add_component<PhysicsBodyComp>(0.2f, (Vector3){0.0f,0.0f,0.0f},(Vector3){0.0f,0.0f,0.0f}, true, false);
    player.add_component<PrimitiveComp>(PrimitiveComp::Shape::CUBE, BLACK,PRIMITVE_WIREFRAME);
    main_camera = player.uuid();


    Entity text = create_entity();
    text.add_component<TagComp>("Text");
    text.add_component<TextComp>("", 40, BLACK);
    text.add_component<TransformComp>((Vector3){0.0f,2.0f,0.0});
    text.add_component<ActionsComp>(ActionsComp().add(new FPSAction(player.uuid()), "FPSAction"));
    text.add_component<LuaActionComp>(
            LuaActionComp(text.uuid())
                 .add(this,"res/scripts/test.lua").bind_field("player", player.uuid())
            );


    Entity plane = create_entity();
    plane.add_component<TagComp>("Floor");
    plane.add_component<TransformComp>((Vector3){(float)0.0f,-0.5f,0.0f},(Vector3){(float)1000.0f,0.0f,1000.0f});
    plane.add_component<PrimitiveComp>(PrimitiveComp::Shape::PLANE, DARKGRAY, PRIMITVE_IMMUNE | PRIMITVE_FILLED);
    plane.add_component<PhysicsBodyComp>(0.0f, (Vector3){0.0f,0.0f,0.0f},(Vector3){0.0f,0.0f,0.0f}, true, true);

    int lim = 10;
    for (int i = 0; i < lim; ++i) {
        Entity entity = create_entity();
        entity.add_component<TagComp>("Cube");
        entity.add_component<TransformComp>((Vector3){(float)i * 3.0f,(float)i* 0.5f,0.0f});
        entity.add_component<PrimitiveComp>(PrimitiveComp::Shape::CUBE, ColorFromHSV(GetRandomValue(0, 360), 1.0f,1.0f), PRIMITVE_FILLED | PRIMITVE_WIREFRAME);
        auto& ph = entity.add_component<PhysicsBodyComp>(0.0f, (Vector3){0.0f,0.0f,0.0f},(Vector3){0.0f,0.0f,0.0f}, false, true);
        entity.add_component<ActionsComp>(ActionsComp().add("CubeAction"));
        if (i == lim -1) {
            ph.is_solid = false;
            //ph.lua_callback = {"res/scripts/fps.lua", "inter"};
        }
    }
    lua_action_init(this);
    manager->write_scene_to_file("res/main.scene", this);
}

void MainScene::on_update(float dt) {
    engine::actions(this, dt);
    engine::lua_action_update(this, dt);
    engine::physics(registry, dt);
    if(engine::aabb_check(*this, dt)) return;
    engine::renderer(player, registry);
}

void MainScene::on_end() {
    lua_action_end(this);
    EG_TRACE("main end called");
}

bool MainScene::should_close() {
    return WindowShouldClose();
}


