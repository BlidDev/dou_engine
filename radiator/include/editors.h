#pragma once
#include "RTscene.h"
#include "state.h"
#include <epch.h>
#include "imgui.h"


using namespace engine;

struct Frambuffer {
    unsigned int handler;
    unsigned int rbo;
    unsigned int texture;
    ImVec2 last_scale;
};

struct ResourceLists {
   std::vector<std::string> scenes;
   std::vector<std::string> shaders;
   std::vector<std::string> textures;
   std::vector<std::string> models;
   std::vector<std::string> scripts;

   ResourceLists();
   void init(SceneManager* manager);
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

    void make_viewer();

    void init_imgui();
    EditorState update_imgui(float dt);
    void end_imgui();

    void render_entity(Entity current, bool* has_selected, bool root = false);
    void render_entities(bool* has_selected);

    void render_overview(bool is_selected);

    void render_editorview(float dt);
    void render_pickerview();
    void render_resources();
    entt::entity entity_from_view(ImVec2 pos, ImVec2 size);

    void render_psettings();

    void save_project();
    Scene* create_scene(const char* name);
    void render_create_scene();

    bool is_key(int k, int a);
    

public:
    Scene* working_scene;
    static int key_query[GLFW_KEY_LAST - 32];
    std::string save_path;

    bool show_project_settings;
    ResourceLists resource_lists;
private:
    bool debug_open;
    bool close;
    UUID selected;

    Frambuffer editorview;

    Frambuffer pickerview;
    Shader picker_shader;
    Entity viewer;

    bool creating_scene;
};

void open_working_file(SceneManager* manager, RTScene* working_scene, EScene* editor);
void save_working_file(SceneManager* manager, EScene* editor);
void saveas_working_file(SceneManager* manager, EScene* editor);


struct RootReseter {
    RootReseter(ProjectData* v) {
        data = v;
        backup = data->root_path;
        data->root_path = "";
    }

    ~RootReseter() {
        data->root_path = backup;
    }

    ProjectData* data;
    std::filesystem::path backup;
};
