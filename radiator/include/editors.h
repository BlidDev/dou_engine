#pragma once
#include "RTscene.h"
#include "state.h"
#include <epch.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>


using namespace engine;


struct ResourceLists {
   std::vector<std::string> scenes;
   std::vector<std::string> shaders;
   std::vector<std::string> textures;
   std::vector<std::string> models;
   std::vector<std::string> scripts;

   ResourceLists();
   void init(SceneManager* manager);

   void refresh_textures(SceneManager* manager);
};


struct SceneSetting {
    SceneSetting() { flag = false; ptr = nullptr; name_backup = {};}
    operator bool() const {return flag && ptr;}
    void on(Scene* ptr) { this->ptr = ptr;  flag = true; name_backup = ptr->name;} 
    void off() { this->ptr = nullptr;  flag = false; name_backup = {};} 
    void render_if_on();
private:
    Scene* ptr;
    bool flag;
    std::string name_backup;
};


struct EditorViewer {

    enum State {
        Natual,
        UsingGizmo,
        FPS,
    };

    EditorViewer() {
        state = State::Natual;
    }

    State state;
};

class EScene : public Scene {
public:
    EScene();

    // Mandatory ======================================== 
    void on_create();
    void on_update(float dt);
    void on_end();
    bool should_close(); 

    // Imgui Loop ======================================= 
    void init_imgui();
    EditorState update_imgui(float dt);
    void end_imgui();
    
    // Setup     ======================================== 
    void make_viewer();
    void setup_imguizmo(ImVec2 pos, ImVec2 size);



    // Rendering ======================================== 
    void render_entity(Entity current, bool* has_selected, bool root = false);
    void render_entities(bool* has_selected);
    void render_prj_settings();
    void render_overview(bool is_selected);
    void render_resources();


    // Editor View ====================================== 

    void render_editorview(float dt);
    void render_grid(ImVec2 pos, ImVec2 size);
    void render_gizmo(ImVec2 pos, ImVec2 size);
    void render_hitboxes();
    void render_pickerview();
    entt::entity entity_from_view(ImVec2 pos, ImVec2 size);
    void pick_entity(ImRect view_rect);



    // Save And Read ==================================== 
    void save_project();
    Scene* create_scene(const char* name);
    void render_create_scene();



    EditorViewer::State& get_editorviewer_state();
    

public:
    Scene* working_scene;
    std::string save_path;

    ResourceLists resource_lists;
private:
    ImFont* icon_font;

    bool debug_open;
    bool close;
    UUID selected;


    bool creating_scene;
    bool editorview_looking;
    bool show_project_settings;
    SceneSetting show_scene_settings;


    Frambuffer pickerview;
    Shader picker_shader;
    Entity viewer;

    ImGuizmo::OPERATION guizmo_operation;
    ImGuizmo::MODE guizmo_mode;
    bool gizmo_snap;
    bool gizmo_fine;
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
