#include "components/transform.h"
#include "editors.h"
#include "helper.h"
#include "imgui.h"
#include "tinyfiledialogs.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"

#define DUMMY ImGui::Dummy(ImVec2(0, 22.0f))
#define NOSPACE

#define PUSH_CMP(vec, entity, type) if(!entity.has_component<type>()) {vec.push_back(std::format("{}", #type));}
std::vector<std::string> make_cmp_vec(Entity& e) {
    std::vector<std::string> vec;
    PUSH_CMP(vec, e, TagComp);
    PUSH_CMP(vec, e, TransformComp);
    PUSH_CMP(vec, e, PhysicsBodyComp);
    PUSH_CMP(vec, e, CameraComp);
    PUSH_CMP(vec, e, LuaActionComp);
    PUSH_CMP(vec, e, ModelComp);
    PUSH_CMP(vec, e, DirLightComp);
    PUSH_CMP(vec, e, PntLightComp);
    PUSH_CMP(vec, e, SptLightComp);
    return vec;
}

void str_to_cmp(Entity& e, std::string& name, Scene* scene) {
    if(name == "TagComp") { e.add_component<TagComp>("Unnamed"); }
    else if(name == "TransformComp") { e.add_component<TransformComp>(); }
    else if(name == "PhysicsBodyComp") { e.add_component<PhysicsBodyComp>(); }
    else if(name == "CameraComp") { e.add_component<CameraComp>(
            CameraBuilder().up({0,1,0}).target({0,0,1}).fovy(70).build()
            ); }
    else if(name == "LuaActionComp") { e.add_component<LuaActionComp>(e.uuid()); }
    else if(name == "ModelComp") { e.add_component<ModelComp>(scene->get_model("cube"), MaterialBuilder().set_shader(scene->get_shader("res/shaders/basic.glsl")).set_color(glm::vec3(1.0f))); }
    else if(name == "DirLightComp") { e.add_component<DirLightComp>(); }
    else if(name == "PntLightComp") { e.add_component<PntLightComp>(); }
    else if(name == "SptLightComp") { e.add_component<SptLightComp>(); }
    else { DU_ASSERT(true, "Unkown component name [{}]", name); }

}

void add_comp(Entity& entity, Scene* scene) {
    ImGui::PushID(entity.uuid());
    ImGui::SetCursorPosX(get_centered_pos("Add Component") / 2.0f);
    std::vector<std::string> cmp_vec = make_cmp_vec(entity);
    std::string current = "NONE";

    if( ImGui::BeginCombo("##", "Add Component") ) {
        for (auto& e : cmp_vec) {
            bool is_selected = current == e;
            if (ImGui::Selectable(e.c_str(), is_selected)) {
                current = e;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();

    if (current == "NONE") 
        return;

    str_to_cmp(entity, current, scene);
}


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


void render_transform(TransformComp& t) {
    sameline_v3("Position", t.position);
    sameline_v3("Rotation", t.rotation);
    sameline_v3("Size    ", t.size);
    if (ImGui::Button("Reset Transform")) {
        t = TransformComp();
    }
}

void apply_delta_on_children(Entity& e, glm::vec3 delta) {
    for (auto child : e.get_children()) {
        Entity tmp = e.scene_ptr()->uuid_to_entity(child);

        if (!tmp.has_component<TransformComp>()) continue;
        tmp.get_component<TransformComp>().position += delta;

        if (!tmp.is_parent()) continue;
        apply_delta_on_children(tmp, delta);
    }

}

void try_transform(Entity& e){
  if (e.has_component<TransformComp>()) {                                          
    if (ImGui::CollapsingHeader("TransformComp",ImGuiTreeNodeFlags_DefaultOpen)) {

        ImGui::Indent();
        auto id=e.uuid()+typeid(TransformComp).hash_code();
        ImGui::PushID(id);
        auto& t = e.get_component<TransformComp>();
        glm::vec3 last = t.position;
        render_transform(t);
        if (t.position != last && e.is_parent()) {
            apply_delta_on_children(e, t.position - last);
        }
        cmp_other_options<TransformComp>(e);
        ImGui::Unindent();
        ImGui::Separator();
        DUMMY;
        ImGui::PopID();
    }                                                                          
  }
}



void render_physicsbody(PhysicsBodyComp &p, Entity &e) {
    sameline_float("Gravity", &p.gravity);
    ImGui::Checkbox("Is Solid", &p.is_solid);
    ImGui::Checkbox("Is Static", &p.is_static);

    if(ImGui::TreeNode("Lua Callback")) {
        ImGui::Text("Current: \"%s\"", p.lua_callback.path.c_str());
        ImGui::SameLine(0, 10.0f); 
        if (ImGui::Button("Set")) {
            char const * filter[] = {"*.lua"};
            char* path = tinyfd_openFileDialog("Select Script file", nullptr, 1, filter, nullptr,0);
            p.lua_callback.path = path;
            if (e.has_component<LuaActionComp>()) {
                auto& la = e.get_component<LuaActionComp>();
                if (!la.find(path))
                    la.add(e.scene_ptr(),path);
            }
            else {
                auto& la = e.add_component<LuaActionComp>(e.uuid());
                la.add(e.scene_ptr(),path);
            }

        }

        ImGui::Text("Function: \"%s\"", p.lua_callback.function.c_str());
        ImGui::SameLine(0, 10.0f); 
        ImGui::InputText("##", &p.lua_callback.function);
        ImGui::TreePop();
    }

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
        sameline_float("Max Distance", &c.max_distance);
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
    int i = 1, current = 0;
    for (auto& [k,_] : manager->texture_lib) {textures.push_back(k);if(m.material.texture.path == k) {current = i;} i++;}

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
    for (auto& [k,_] : manager->shader_lib) {shaders.push_back(k);if(m.material.shader.path == k) {current = i;}  i++;}

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
    for (auto& [k,_] : manager->model_lib) {models.push_back(k);if(m.model.name == k) {current = i;}  i++;}

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


void render_luascript(LuaActionComp& l, Scene* scene) {
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
            if (ImGui::Button(std::format("Remove##{}", s.path).c_str())) {
                l.remove(s.path);
            }

            ImGui::TreePop();
        }
    }
    if(ImGui::Button("Add Script")) {
        char const * filter[] = {"*.lua"};
        char* path = tinyfd_openFileDialog("Open a Script file", nullptr, 1, filter, nullptr,0);

        l.add(scene, path);
    }
}

void render_parent(ParentComp& p, Scene* scene, Entity& self) {


    std::string name = make_entity_name(p.parent);

    ImGui::Text("%s", name.c_str()); ImGui::SameLine(0.0f, 150.0f);
    if (ImGui::Button(std::format("Remove Parent").c_str())) {
        p.parent.remove_child(self.uuid());
    }
}

void render_children(ChildrenComp& c, Scene* scene, Entity& parent) {
    auto width = ImGui::GetWindowWidth();

    int i = 0;
    for (auto& child : c.children) {
        Entity current = scene->uuid_to_entity(child);

        std::string name = make_entity_name(current);
        auto dis_width = ImGui::CalcTextSize("Disown").x;
        auto text_width = ImGui::CalcTextSize(name.c_str()).x;


        ImGui::Text("* %s", name.c_str()); ImGui::SameLine(); ImGui::SetCursorPosX((width - dis_width) /2); 
        if (ImGui::Button(std::format("Disown##{}",i).c_str())) {
            parent.remove_child(child);
        }
        ++i;
    }

    if (c.children.empty()) {
        parent.remove_component<ChildrenComp>();
    }
}

void add_children(Entity& self, Scene* scene) {
    auto view = scene->registry.view<entt::entity>(entt::exclude<ParentComp>);
    entt::entity current = entt::null;

    ImGui::SetCursorPosX(get_centered_pos("Add Child") / 2.0f);

    if( ImGui::BeginCombo("##", "Add Child") ) {
        for (auto& e : view) {
            if (e == self.id()) continue;;
            Entity tmp = {scene, e};
            bool is_selected = e == current;
            if (ImGui::Selectable(make_entity_name(tmp).c_str(), is_selected)) {
                current = e;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (current == entt::null) 
        return;

    Entity{scene, current}.make_child_of(self.uuid());
}



#define TRY_COMPONENT(entity, type, name, sep, ...)                            \
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
      sep;                                                                     \
      ImGui::PopID();                                                          \
    }                                                                          \
  }
//cmp_other_options<type>(entity);                                           
  
void EScene::render_overview(bool is_selected) {
    ImGui::Begin("Overview", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    if (!is_selected || !selected) {ImGui::End(); return;}

    Entity tmp = working_scene->uuid_to_entity(selected);
    ImGui::Text("UUID: %lu", (uint64_t)tmp.uuid());
    ImGui::SameLine();
    if (ImGui::Button("Copy"))
        ImGui::SetClipboardText(std::format("{}", (uint64_t)tmp.uuid()).c_str());
    

    TRY_COMPONENT(tmp, TagComp, tag, DUMMY);
    try_transform(tmp);

    TRY_COMPONENT(tmp, ChildrenComp, children, NOSPACE,working_scene, tmp);
    add_children(tmp, working_scene);
    DUMMY;
    TRY_COMPONENT(tmp, ParentComp, parent, DUMMY,working_scene, tmp);

    TRY_COMPONENT(tmp, LuaActionComp, luascript, DUMMY, working_scene);
    TRY_COMPONENT(tmp, ModelComp, model, DUMMY, manager);
    TRY_COMPONENT(tmp, PhysicsBodyComp, physicsbody, DUMMY, tmp);

    TRY_COMPONENT(tmp, CameraComp, camera, DUMMY);

    TRY_COMPONENT(tmp, DirLightComp, dirlight, DUMMY);
    TRY_COMPONENT(tmp, PntLightComp, pntlight, DUMMY);
    TRY_COMPONENT(tmp, SptLightComp, sptlight, DUMMY);

    add_comp(tmp, working_scene);

    ImGui::End();
}
