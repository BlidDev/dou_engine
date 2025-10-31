#include "editors.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"


template <typename T>
void cmp_other_options(Entity &e) {
    if (ImGui::CollapsingHeader("Other options")) {
        if (ImGui::Button("Remove component")) {
            e.remove_component<T>();
        }
    }
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

    if (ImGui::TreeNode("Advanced physics options")) {
        sameline_v3("Acceleration", p.acceleration);
        sameline_v3("Velocity    ", p.velocity);
        ImGui::TreePop();
    }
    ImGui::GetStyle().ItemSpacing = ImVec2(0, 10.0f);
    ImGui::Spacing();
}

const char* camera_proj[] = {
        "Perspective",
        "Orthographic"
};

void render_camera(CameraComp& c) {

    sameline_float("FOV", &c.fovy);


    render_combo("Camera Projection", &c.projection, camera_proj, IM_ARRAYSIZE(camera_proj));

    if (ImGui::TreeNode("Advanced camera options")) {
           sameline_v3("Target", c.target);
           sameline_v3("Up    ", c.up);
        sameline_float("Max Distance", &c.fovy);
        ImGui::TreePop();
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

    if (ImGui::TreeNode("Material Args")) {
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
        ImGui::TreePop();
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


void render_luascript(LuaActionComp& l) {
    for (auto& s : l.scripts) {
        if (ImGui::TreeNodeEx(s.path.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (auto& [k,v] : s.env) {
                if ( !k.is<std::string>() || v.is<sol::function>())
                    continue;
                std::string key = k.as<std::string>();

                std::string val = sol::type_name(s.env.lua_state(),v.get_type());
                if (v.is<bool>()) {
                    bool tmp = v.as<bool>();
                    ImGui::Checkbox(std::format("{}: ", key).c_str(), &tmp);
                    s.env[key] = tmp;

                }
                else if (v.is<int>()) {
                    int tmp = v.as<int>();
                    sameline_int(std::format("{}: ", key).c_str(), &tmp);
                    s.env[key] = tmp;
                }
                else if (v.is<float>()) {
                    float tmp = v.as<float>();
                    sameline_float(std::format("{}: ", key).c_str(), &tmp);
                    s.env[key] = tmp;
                }
                else if (v.is<glm::vec2>()) {
                    glm::vec2 tmp = v.as<glm::vec2>();
                    sameline_v2(std::format("{}: ", key).c_str(), tmp);
                    s.env[key] = tmp;
                }
                else if (v.is<glm::vec3>()) {
                    glm::vec3 tmp = v.as<glm::vec3>();
                    sameline_v3(std::format("{}: ", key).c_str(), tmp);
                    s.env[key] = tmp;
                }
            }

            ImGui::TreePop();
        }
    }
}

#define TRY_COMPONENT(entity, type, name, ...)                                 \
  if (entity.has_component<type>()) {                                          \
    if (ImGui::CollapsingHeader(std::format("{}", #type).c_str(),              \
                                ImGuiTreeNodeFlags_DefaultOpen)) {             \
      ImGui::Indent();                                                         \
      auto id =entity.uuid() + typeid(type).hash_code();\
      ImGui::PushID(id);                 \
      render_##name(entity.get_component<type>(), ##__VA_ARGS__);              \
      cmp_other_options<type>(entity);\
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

    Entity tmp = working_scene->uuid_to_entity(selected);
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
    TRY_COMPONENT(tmp, LuaActionComp, luascript);

    ImGui::End();
}
