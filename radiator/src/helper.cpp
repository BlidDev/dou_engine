#include "helper.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"


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



void sameline_color(const char* title, glm::vec3& v) {
    ImGui::Text("%s", title);
    ImGui::SameLine();
    ImGui::ColorEdit3(std::format("##{}", title).c_str(), glm::value_ptr(v));
}

void sameline_v3(const char* title, glm::vec3& v, float min, float max, float speed) {
    ImGui::Text("%s", title);
    ImGui::SameLine();
    ImGui::DragFloat3(std::format("##{}", title).c_str(), glm::value_ptr(v), speed, min, max);
}

void sameline_v2(const char* title, glm::vec2& v, float min, float max, float speed) {
    ImGui::Text("%s", title);
    ImGui::SameLine();
    ImGui::DragFloat2(std::format("##{}", title).c_str(), glm::value_ptr(v), speed, min, max);
}

void sameline_float(const char* title, float* v, float min, float max, float speed) {
    ImGui::Text("%s", title);
    ImGui::SameLine();
    ImGui::DragFloat(std::format("##{}", title).c_str(), v, speed,min, max);
}


void sameline_int(const char* title, int* v, int min, int max, int speed) {
    ImGui::Text("%s", title);
    ImGui::SameLine();
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
