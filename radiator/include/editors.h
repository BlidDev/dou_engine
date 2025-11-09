#pragma once
#include "RTscene.h"
#include "state.h"
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

    void make_viewer();

    void init_imgui();
    EditorState update_imgui(float dt);
    void end_imgui();

    void render_entity(Entity current, bool* has_selected, bool root = false);
    void render_entities(bool* has_selected);


    void render_overview(bool is_selected);

    void render_editorview(float dt);


    bool is_key(int k, int a);
    

public:
    RTScene* working_scene;
    static int key_query[GLFW_KEY_LAST - 32];
    std::string save_path;

    
private:
    bool close;
    UUID selected;

    Frambuffer editorview;
    Entity viewer;

};

void open_working_file(SceneManager* manager, RTScene* working_scene, EScene* editor);
void save_working_file(SceneManager* manager, EScene* editor);
void saveas_working_file(SceneManager* manager, EScene* editor);




void make_framebuffer(Frambuffer& fb, size_t w, size_t h);
void rescale_framebuffer(Frambuffer& fb, size_t w, size_t h);




void sameline_color(const char* title, glm::vec3& v);
void sameline_v3(const char* title, glm::vec3& v, float min = 0.F, float max = 0.F, float speed = 0.1f);
void sameline_v2(const char* title, glm::vec2& v, float min = 0.F, float max = 0.F, float speed = 0.1f);
void sameline_float(const char* title, float* v, float min = 0.F, float max = 0.F, float speed = 0.1f);
void sameline_int(const char* title, int* v, int min = 0.F, int max = 0.F, int speed = 1);

template <typename T>
void render_combo(const char* label, T* subject, const char** index, size_t arr_size) {
    ImGui::Text("%s", label); ImGui::SameLine();
    int current = static_cast<int>(*subject);

    if (ImGui::BeginCombo(std::format("##{}",label).c_str(), index[current])) {
        for (int i = 0; i < arr_size; i++) {
            bool is_selected = (*subject == i);
            if (ImGui::Selectable(index[i], is_selected)) {
                *subject = static_cast<T>(i);
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }



}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


bool check_key_combo(int keys[], int combo_len);
