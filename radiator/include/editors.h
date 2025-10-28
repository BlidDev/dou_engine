#pragma once
#include <epch.h>


using namespace engine;


class EScene : public Scene {
public:
    EScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 


    void init_imgui();
    void update_imgui();
    void end_imgui();

    void render_entity(Entity current, bool* has_selected, bool root = false);
    void render_entities(bool* has_selected);


    void render_overview(bool is_selected);

private:
    bool close;
    Entity player;
    UUID selected;

};
