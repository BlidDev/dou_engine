#include "helper.h"
#include "glm/gtc/type_ptr.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>

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
    io.Fonts->AddFontFromFileTTF("res/fonts/DejaVu.ttf", 16.0f);
    io.WantSetMousePos = true;
    io.FontGlobalScale = 1.5f;
}


void make_framebuffer(Frambuffer& fb, size_t w, size_t h) {
    glGenFramebuffers(1, &fb.handler);
    glBindFramebuffer(GL_FRAMEBUFFER, fb.handler);


    glGenTextures(1, &fb.texture);
    glBindTexture(GL_TEXTURE_2D, fb.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.texture, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &fb.rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, fb.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb.rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        DU_CORE_ERROR("Could not make frame buffer for editor view}");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    fb.last_scale = ImVec2(w,h);
}



void rescale_framebuffer(Frambuffer& fb, size_t w, size_t h) {
    glBindTexture(GL_TEXTURE_2D, fb.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.texture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, fb.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb.rbo);

    fb.last_scale = ImVec2(w,h);
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
