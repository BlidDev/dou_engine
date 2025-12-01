#include "helper.h"
#include "glm/gtc/type_ptr.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include <IconsMaterialDesign.h>

void initialize_imgui(SceneManager* manager) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(manager->main_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/DejaVu.ttf", 16.0f);
    io.WantSetMousePos = true;

    const auto video_mod = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (video_mod->height > 1080)
        io.FontGlobalScale = 1.5f;

    ImFontConfig icon_font_cfg;
    icon_font_cfg.PixelSnapH = true;
    icon_font_cfg.MergeMode = true;
    static const ImWchar icons_ranges[] = { ICON_MIN_MD,ICON_MAX_16_MD, 0 };
    ImGui::GetIO().Fonts->AddFontFromFileTTF("res/fonts/Google-MD-Regular.ttf", 16.0f, &icon_font_cfg, icons_ranges);

    ImGui::GetIO().Fonts->Build();
    
}




void sameline_text(const char* title, std::string* v) {
    ImGui::Text("%s", title);
    ImGui::SameLine(0.0f, 10.0f);
    ImGui::InputText(std::format("##{}", title).c_str(), v);
}

void sameline_checkbox(const char* title, bool* v) {
    ImGui::Text("%s", title);
    ImGui::SameLine(0.0f, 10.0f);
    ImGui::Checkbox(std::format("##{}", title).c_str(), v);
}

void sameline_color(const char* title, glm::vec4& v) {
    ImGui::Text("%s", title);
    ImGui::SameLine(0.0f, 10.0f);
    ImGui::ColorEdit4(std::format("##{}", title).c_str(), glm::value_ptr(v));
}

void sameline_color(const char* title, glm::vec3& v) {
    ImGui::Text("%s", title);
    ImGui::SameLine(0.0f, 10.0f);
    ImGui::ColorEdit3(std::format("##{}", title).c_str(), glm::value_ptr(v));
}

void sameline_v3(const char* title, glm::vec3& v, float min, float max, float speed) {
    ImGui::Text("%s", title);
    ImGui::SameLine(0.0f, 10.0f);
    ImGui::DragFloat3(std::format("##{}", title).c_str(), glm::value_ptr(v), speed, min, max);
}

void sameline_v2(const char* title, glm::vec2& v, float min, float max, float speed) {
    ImGui::Text("%s", title);
    ImGui::SameLine(0.0f, 10.0f);
    ImGui::DragFloat2(std::format("##{}", title).c_str(), glm::value_ptr(v), speed, min, max);
}

void sameline_float(const char* title, float* v, float min, float max, float speed) {
    ImGui::Text("%s", title);
    ImGui::SameLine(0.0f, 10.0f);
    ImGui::DragFloat(std::format("##{}", title).c_str(), v, speed,min, max);
}


void sameline_int(const char* title, int* v, int min, int max, int speed) {
    ImGui::Text("%s", title);
    ImGui::SameLine(0.0f, 10.0f);
    ImGui::DragInt(std::format("##{}", title).c_str(), v, speed,min, max);
}




bool combo_guts(const char* label, const std::vector<std::string>& tmp,int& current, int safety) {
    std::string sample = (current == safety) ? "##" : tmp[current];
    if (ImGui::BeginCombo(label, sample.c_str())) {
        for (int i = 0; i < tmp.size(); i++) {
            bool is_selected = (current == i);
            if (ImGui::Selectable(tmp[i].c_str(), is_selected)) {
                current = i;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
        if (current == safety) return false;
        return true;
    }
    return false;
}




void render_str_select(const char* label, std::string& subject, std::vector<std::string>&list) {
    const auto& it = std::find(list.begin(), list.end(), subject);
    int current = (subject.empty() || it == list.end()) ? -1 : std::distance(list.begin(), it);

    if (combo_guts(label, list, current)) {
        subject = list[current];
    }
}

bool EScene::is_key(int k, int a) {
    return key_query[k - 32] == a;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    EScene::key_query[key-32] = action;
}

bool check_key_combo(int keys[], int combo_len) {
    for (int i = 0; i < combo_len; i++) {
        if (!is_key_pressed(keys[i]))
            return false;
    }

    return true;
}

std::string make_entity_name(Entity &entity) {
    std::string name = (entity.has_component<TagComp>()) ? 
                        entity.get_component<TagComp>().tag :
                        std::format("Unnamed entity {}...", entity.uuid() >> 16);
    return name;
}


float get_centered_pos(const char* text) {
    auto width = ImGui::GetWindowWidth();
    auto text_width = ImGui::CalcTextSize(text).x;

    return ((width - text_width) / 2.0f);
}
