#pragma once
#include <epch.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "imgui.h"


using namespace engine;

struct Frambuffer {
    unsigned int handler;
    unsigned int rbo;
    unsigned int texture;
    ImVec2 last_scale;
};


struct EditorViewer {
    EditorViewer() : thing(0) {}

private:
    int thing = 0;
};

class EScene : public Scene {
public:
    EScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 


    void init_imgui();
    void update_imgui(float dt);
    void end_imgui();

    void render_entity(Entity current, bool* has_selected, bool root = false);
    void render_entities(bool* has_selected);


    void render_overview(bool is_selected);

    void render_editorview(float dt);
private:
    bool close;
    UUID selected;

    Frambuffer editorview;
    Entity viewer;
};


void make_framebuffer(Frambuffer& fb, size_t w, size_t h);
void rescale_framebuffer(Frambuffer& fb, size_t w, size_t h);
