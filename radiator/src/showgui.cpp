#include "editors.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"

void EScene::init_imgui() {

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
    // io.FontGlobalScale = 1.5f;
}

void EScene::update_imgui(float dt) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open"))
            EG_ERROR("dude we dont have that kind of budget");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // Create a dockspace
    ImGui::DockSpaceOverViewport();

    bool has_selected = false;
    render_entities(&has_selected);
    render_overview(has_selected);

    render_editorview(dt);

    // Rendering
    ImGui::Render();
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EScene::end_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EScene::render_entities(bool *has_selected) {

    ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto [_, entity] : uuids) {
        Entity tmp = {this, entity};
        if (tmp.is_child() || tmp.has_component<EditorViewer>())
            continue;
        render_entity(tmp, has_selected, true);
    }
    ImGui::End();
}

void EScene::render_entity(Entity current, bool *has_selected, bool root) {
    ImGuiTreeNodeFlags flags = 0;
    if (root) flags |= ImGuiTreeNodeFlags_DefaultOpen;

    if (!current.is_parent())flags |= ImGuiTreeNodeFlags_Leaf;


    std::string name = (current.has_component<TagComp>()) ? 
                        current.get_component<TagComp>().tag :
                        std::format("Unnamed entity {}...", current.uuid() >> 16);

    flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

    if (selected == current.uuid()) {
        flags |= ImGuiTreeNodeFlags_Selected;
        *has_selected = true;
    }

    if (ImGui::TreeNodeEx(name.c_str(), flags)) {
        if (ImGui::IsItemClicked()) {
            selected = current.uuid();
        }

        if (current.is_parent()) {
            for (auto child : current.get_children()) {
                render_entity(uuid_to_entity(child), has_selected);
            }
        }

        ImGui::TreePop();
    }
}

void sameline_color(const char* title, glm::vec3& v) {
    ImGui::Text("%s", title);
    ImGui::SameLine();
    ImGui::ColorEdit3(std::format("##{}", title).c_str(), glm::value_ptr(v));
}

void sameline_v3(const char* title, glm::vec3& v, float min = 0.F, float max = 0.F, float speed = 0.1f) {
    ImGui::Text("%s", title);
    ImGui::SameLine();
    ImGui::DragFloat3(std::format("##{}", title).c_str(), glm::value_ptr(v), speed, min, max);
}

void sameline_float(const char* title, float* v, float min = 0.F, float max = 0.F, float speed = 0.1f) {
    ImGui::Text("%s", title);
    ImGui::SameLine();
    ImGui::DragFloat(std::format("##{}", title).c_str(), v, speed,min, max);
}

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

template <typename T>
void cmp_other_options(Entity &e) {
    ImGui::PushID(rand() % e.uuid());
    if (ImGui::CollapsingHeader("Other options")) {
        if (ImGui::Button("Remove component")) {
            e.remove_component<T>();
        }
    }
    ImGui::PopID();
}

void render_tag(TagComp &tag) {
    ImGui::Text("Tag: "); ImGui::SameLine();
    ImGui::InputText("##Tag", &tag.tag);
    if (tag.tag.empty()) {
        tag.tag = "Unnamed";
    }
}

void render_transform(TransformComp &t) {
    sameline_v3("Position", t.position);
    sameline_v3("Rotation", t.rotation);
    sameline_v3("Size    ", t.size);
    if (ImGui::Button("Reset Transform")) {
        t = TransformComp();
    }
}

void render_physicsbody(PhysicsBodyComp &p) {
    sameline_float("Gravity", &p.gravity);
    ImGui::Checkbox("Is Solid", &p.is_solid);
    ImGui::Checkbox("Is Static", &p.is_static);

    if (ImGui::CollapsingHeader("Advanced physics options")) {
        sameline_v3("Acceleration", p.acceleration);
        sameline_v3("Velocity    ", p.velocity);
    }
}

const char* camera_proj[] = {
        "Perspective",
        "Orthographic"
};

void render_camera(CameraComp& c) {

    sameline_float("FOV", &c.fovy);


    render_combo("Camera Projection", &c.projection, camera_proj, IM_ARRAYSIZE(camera_proj));

    if (ImGui::CollapsingHeader("Advanced camera options")) {
           sameline_v3("Target", c.target);
           sameline_v3("Up    ", c.up);
        sameline_float("Max Distance", &c.fovy);
    }

}

const char* num_to_str[MAX_RENDER_LAYERS] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};

bool str_vec_getter(void* data, int n, const char** out_text)
{
    const std::vector<std::string>* v = static_cast<std::vector<std::string>*>(data);
    *out_text = v->at(n).c_str();
    return true;
}

void render_tex_select(ModelComp& m, SceneManager* manager) {

    ImGui::Text("Texture"); ImGui::SameLine();
    std::vector<std::string>textures = {"UNKNOWN"};
    int i = 0, current = 0;
    for (auto& [k,_] : manager->texture_lib) {textures.push_back(k);}
    for (auto& k : textures) {if(m.material.texture.path == k) {current = i; break;}  i++;}

    std::string last = textures[current];
    if (ImGui::BeginCombo("##Texture", last.c_str())) {
        for (int i = 0; i < textures.size(); i++) {
            bool is_selected = (current == i);
            if (ImGui::Selectable(textures[i].c_str(), is_selected)) {
                current = i;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();

        if (textures[current] == last) return;
        if (textures[current] == "UNKNOWN") {
            m.material.texture = Texture();
            m.material.attributes &= ~MODEL_TEXTURED;
            m.material.attributes |= MODEL_FILLED;
            return;
        }

        m.material.texture = manager->texture_lib[textures[current]];
        m.material.attributes |= MODEL_TEXTURED;
        m.material.attributes &= ~MODEL_FILLED;
    }
}


void render_shader_select(ModelComp& m, SceneManager* manager) {

    ImGui::Text("Shader "); ImGui::SameLine();
    std::vector<std::string>shaders;
    int i = 0, current = 0;
    for (auto& [k,_] : manager->shader_lib) {shaders.push_back(k);}
    for (auto& k : shaders) {if(m.material.shader.path == k) {current = i; break;}  i++;}

    std::string last = shaders[current];
    if (ImGui::BeginCombo("##Shader", last.c_str())) {
        for (int i = 0; i < shaders.size(); i++) {
            bool is_selected = (current == i);
            if (ImGui::Selectable(shaders[i].c_str(), is_selected)) {
                current = i;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
        if (shaders[current] == last) return;

        m.material.shader = manager->shader_lib[shaders[current]];

    }
}

void render_model_select(ModelComp& m, SceneManager* manager) {

    ImGui::Text("Model  "); ImGui::SameLine();
    std::vector<std::string>models;
    int i = 0, current = 0;
    for (auto& [k,_] : manager->model_lib) {models.push_back(k);}
    for (auto& k : models) {if(m.model.name == k) {current = i; break;}  i++;}

    std::string last = models[current];
    if (ImGui::BeginCombo("##Model", last.c_str())) {
        for (int i = 0; i < models.size(); i++) {
            bool is_selected = (current == i);
            if (ImGui::Selectable(models[i].c_str(), is_selected)) {
                current = i;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
        if (models[current] == last) return;

        m.model = manager->model_lib[models[current]];

    }
}

void render_model(ModelComp& m, SceneManager* manager) {
    render_combo("Render Layer", &m.layer, num_to_str, MAX_RENDER_LAYERS);

    render_shader_select(m, manager);
    render_model_select(m,manager);
    render_tex_select(m, manager);

    if (ImGui::CollapsingHeader("Material Args")) {
        sameline_color("Ambient ", m.material.ambient);
        sameline_color("Diffuse ", m.material.diffuse);
        sameline_color("Specular", m.material.specular);
        sameline_float("Shininess", &m.material.shininess, 0.0f, 1.0f);

        bool is_filled = (m.material.attributes & MODEL_FILLED) == MODEL_FILLED;
        bool is_textured = (m.material.attributes & MODEL_TEXTURED) == MODEL_TEXTURED;
        bool is_immune = (m.material.attributes & MODEL_IMMUNE) == MODEL_IMMUNE;

        ImGui::Checkbox("Filled", &is_filled);
        ImGui::Checkbox("Textured", &is_textured);
        ImGui::Checkbox("Immune", &is_immune);

        m.material.attributes = ((is_filled)   ? MODEL_FILLED   : 0) |
                                ((is_textured) ? MODEL_TEXTURED : 0) |
                                ((is_immune)   ? MODEL_IMMUNE   : 0);
    }

}


void render_dirlight(DirLightComp& d) {
    sameline_v3   ("Direction", d.direction, -1.0f, 1.0f);
    sameline_color("Color    ", d.color);
    sameline_float("Strength ", &d.strength, 0.0, 1.0f);
    sameline_float("Ambient  ", &d.ambient, 0.0, 1.0f);
}

void render_pntlight(PntLightComp& p) {
    sameline_color("Color    ", p.color);
    sameline_float("Constant ", &p.constant, 0.0, 1.0f);
    sameline_float("Linear   ", &p.linear, 0.0, 1.0f);
    sameline_float("Quadratic", &p.quadratic, 0.0, 1.0f);
}

void render_sptlight(SptLightComp& s) {

    sameline_v3   ("Direction   ",  s.direction, -1.0f, 1.0f);
    sameline_color("Color       ",  s.color);
    sameline_float("Cutoff      ", &s.cutoff, 0.0, 1.0f);
    sameline_float("Outer Cutoff", &s.outer_cutoff, 0.0, 1.0f);
    sameline_float("Constant    ", &s.constant, 0.0, 1.0f);
    sameline_float("Linear      ", &s.linear, 0.0, 1.0f);
    sameline_float("Quadratic   ", &s.quadratic, 0.0, 1.0f);
    
}

#define TRY_COMPONENT(entity, type, name, ...)                                 \
  if (entity.has_component<type>()) {                                          \
    if (ImGui::CollapsingHeader(std::format("{}", #type).c_str(),              \
                                ImGuiTreeNodeFlags_DefaultOpen)) {             \
      ImGui::Indent();                                                         \
      ImGui::PushID(entity.uuid() + typeid(type).hash_code());                 \
      render_##name(entity.get_component<type>(), ##__VA_ARGS__);              \
      ImGui::Unindent();                                                       \
      ImGui::Separator();                                                      \
      ImGui::Spacing();                                                        \
      ImGui::PopID();                                                          \
    }                                                                          \
  }
//cmp_other_options<type>(entity);                                           \

void EScene::render_overview(bool is_selected) {
    ImGui::Begin("Overview", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    if (!is_selected) {ImGui::End(); return;}

    Entity tmp = uuid_to_entity(selected);
    ImGui::Text("UUID: %lu", (uint64_t)tmp.uuid());
    ImGui::SameLine();
    if (ImGui::Button("Copy"))
        ImGui::SetClipboardText(std::format("{}", (uint64_t)tmp.uuid()).c_str());

    TRY_COMPONENT(tmp, TagComp, tag);
    TRY_COMPONENT(tmp, TransformComp, transform);
    TRY_COMPONENT(tmp, PhysicsBodyComp, physicsbody);
    TRY_COMPONENT(tmp, ModelComp, model, manager);

    TRY_COMPONENT(tmp, DirLightComp, dirlight);
    TRY_COMPONENT(tmp, PntLightComp, pntlight);
    TRY_COMPONENT(tmp, SptLightComp, sptlight);

    ImGui::End();
}

void wrape_mouse_in_window(GLFWwindow* window, ImVec2 window_size, ImVec2 window_pos) {
    ImVec2 pos = ImGui::GetMousePos();
    glm::vec2 last = {pos.x, pos.y};
    if(pos.x >= window_size.x + window_pos.x)
        pos.x = window_pos.x;

    if(pos.y >= window_size.y + window_pos.y)
        pos.y = window_pos.y;

    if (pos.x < window_pos.x)
        pos.x = window_pos.x + window_size.x - 1;

    if (pos.y < window_pos.y)
        pos.y = window_pos.y + window_size.y - 1;

    glm::vec2 current = {pos.x, pos.y};
    if (current == last)
        return;

    glfwSetCursorPos(window, pos.x, pos.y);
}

void EScene::render_editorview(float dt) {

    ImGui::Begin("Editor View");
    ImVec2 size =ImGui::GetContentRegionAvail();
    ImVec2 pos = ImGui::GetCursorScreenPos();

    static bool looking = false;
    if ((ImGui::IsWindowHovered() || looking) && is_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {

        glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        //wrape_mouse_in_window(manager->main_window, size, pos);
        auto& lua = viewer.get_component<LuaActionComp>();
        lua
            .bind_field("thing", 0)
            .bind_field("mouse_delta", glm::vec2(0.0f));
        lua.get_last().on_update(dt);

        looking = true;
    }
    else {
        looking = false;
        glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        auto& lua = viewer.get_component<LuaActionComp>();
        lua.bind_field("first", true);
    }

    if (glm::vec2(size.x, size.y) != glm::vec2(editorview.last_scale.x, editorview.last_scale.y))
        rescale_framebuffer(editorview, size.x, size.y);

    glViewport(0,0, size.x, size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, editorview.handler);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    opengl_renderer(manager->render_data,{size.x, size.y}, viewer, registry, true);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::GetWindowDrawList()->AddImage(
        (void*)(intptr_t)editorview.texture,
        pos,
        ImVec2(pos.x + size.x, pos.y + size.y),  
        ImVec2(0, 1),
        ImVec2(1, 0)
    );


    ImGui::End();
}

