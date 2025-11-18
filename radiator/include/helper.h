#pragma once
#include "editors.h"

void initialize_imgui(SceneManager* manager);

void make_framebuffer(Frambuffer& fb, size_t w, size_t h);
void rescale_framebuffer(Frambuffer& fb, size_t w, size_t h);




void sameline_text(const char* title, std::string* v);
void sameline_checkbox(const char* title, bool* v);
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

std::string make_entity_name(Entity& entity);


float get_centered_pos(const char* text);

