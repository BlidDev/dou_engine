#include "components/modelcomp.h"
#include "helper.h"
#include "imgui_internal.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>

#include <tinyfiledialogs.h>
#include <IconsMaterialDesign.h>

extern ImGuiContext* GImGui;

void EScene::init_imgui() {
    if (!GImGui) initialize_imgui(manager);
}


void add_to_working_file(SceneManager* manager, Scene* working_scene, EScene* editor){
    char* path = tinyfd_openFileDialog("Add From A Scene File", nullptr, 0, nullptr, nullptr,0);
    if(!path) return;
    manager->clear_scene(working_scene);
    working_scene->add_from_file(path);
}

void saveas_working_file(SceneManager* manager, EScene* editor) {
    char* path= tinyfd_saveFileDialog("Save Current Scene As", nullptr, 0, nullptr, nullptr);
    if (!path) return;
    manager->write_scene_to_file(path, editor->working_scene); 
    DU_TRACE("Saving to [{}]", path);
}



void EScene::save_project() {
    ProjectData& data = manager->project_data;
    if(data.root_path.empty()) {
        const char* tmp_path = tinyfd_selectFolderDialog("Select Directory To Save Project", std::filesystem::current_path().c_str());
        if (!tmp_path) return;
        data.root_path = tmp_path;
    }

    if (data.scene_paths.empty()) {data.scene_paths.push_back(data.root_path);}
    for (const auto& [k, s] : manager->get_scenes()) {
        if (k == "EDITOREditor" || k == "EDITORGreeter") continue;
        if (!s->file_path.empty() && s->uuids.empty()) continue; // exists but isn't loaded yet
        std::string path = s->file_path;
        if (path.empty())
            path = std::format("{}/{}/{}.scene", data.root_path.string(), data.scene_paths[0].string(), s->name);
        manager->write_scene_to_file(path.c_str(), s.get());
    }
    std::string prj_path = std::format("{}/{}.prj", data.root_path.string(), data.name).c_str();

    write_project_file(prj_path.c_str(), data, manager->render_data.layers_atrb.data());
}

size_t counter = 0;

EditorState EScene::update_imgui(float dt) {

    bool is_working = working_scene;
    const auto& clear_color = (is_working) ? working_scene->s_render_data.clear_color : glm::vec4{0.5f, 0.5f, 0.5f, 1.0f};
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    EditorState state = EditorState::EditorNormal;

    counter++;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    int _open[] = {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_O};
    int _save[] = {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_S};
    int _saveas[] = {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_S};

    if (!is_working) counter = 0;

    if (get_editorviewer_state() != EditorViewer::State::Natual) counter = 0;

    if (check_key_combo(_open, 2) && counter > 10){
        add_to_working_file(manager, working_scene, this);
        counter = 0;
    }
    else if (check_key_combo(_save, 2) && counter > 10){
        save_project();
        counter = 0;
    }
    else if (check_key_combo(_saveas, 3) && counter > 10){
        saveas_working_file(manager, this);
        counter = 0;
    }
    else if (is_key_pressed(GLFW_KEY_F5) && counter > 10) {
        state = EditorState::EditorPreview;
        counter = 0;
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {

            if (ImGui::MenuItem("Save Project")) {
                save_project();
            }
            if (ImGui::MenuItem("Import From Scene", "Ctrl + O")) {
                add_to_working_file(manager, working_scene, this);
            }
            if (ImGui::MenuItem("Extract Current Scene", "Ctrl + Shift + S")) {
                saveas_working_file(manager,this);
            }
            if(ImGui::MenuItem("New Scene")) {
                creating_scene = true;
            }

            if (ImGui::MenuItem("Project Settings")) {
                show_project_settings = true;
            }


            if (ImGui::MenuItem("Quit")) close = true;
            ImGui::EndMenu();
        }

        ImGui::Spacing();
        if (ImGui::Button(ICON_MD_SAVE "##SaveProject")) {
            save_project();
        }
        ImGui::Spacing();
        if (ImGui::ArrowButton("Play", ImGuiDir::ImGuiDir_Right)) {
            state = EditorState::EditorPreview;
        }
        


        ImGui::EndMainMenuBar();
    }


    // Create a dockspace
    ImGui::DockSpaceOverViewport();

    bool has_selected = false;
    if (working_scene) {
        render_editorview(dt);
        if (editorview_looking) {
            ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
        }
        render_resources();
        render_entities(&has_selected);
        render_overview(has_selected);
    }
    if (show_project_settings)
        render_prj_settings();

    show_scene_settings.render_if_on();

    if (creating_scene)
        render_create_scene();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return state;
}

void EScene::end_imgui() {
}

void EScene::render_entities(bool *has_selected) {
    ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {

        if (ImGui::BeginMenu("New")) {
            if( ImGui::MenuItem("Empty Entity") ) {
                auto e = working_scene->create_entity();
                e.add_component<TagComp>("Unnamed");
                if (selected) {
                    e.make_child_of(selected);
                }
                selected = e.uuid();
            }

            if( ImGui::MenuItem("Cube") ) {
                auto e = working_scene->create_entity();
                e.add_component<TagComp>("Unnamed Cube");
                e.add_component<TransformComp>();
                e.add_component<ModelComp>(working_scene->get_model("cube"), 
                                           MaterialBuilder()
                                                           .set_shader(working_scene->get_shader("basic.glsl"))
                                                           .set_color(glm::vec3(1.0f)));
                if (selected) {
                    e.make_child_of(selected);
                }
                selected = e.uuid();
            }

            ImGui::EndMenu();
        }

        if (selected) {
            ImGui::SameLine(0, 10.0f);
            if (ImGui::Button("Delete")) {
                working_scene->remove_entity(selected);
                selected = 0;
                *has_selected = false;
            }
        }

        ImGui::EndMenuBar();
    }
    

    for (const auto& [_, entity] : working_scene->uuids) {
        Entity tmp = {working_scene, entity};
        if (tmp.is_child())
            continue;
        ImGui::PushID(tmp.uuid());
        render_entity(tmp, has_selected, true);
        ImGui::PopID();
    }
    ImGui::End();
}

void EScene::render_entity(Entity current, bool *has_selected, bool root) {
    ImGuiTreeNodeFlags flags = 0;
    if (root) flags |= ImGuiTreeNodeFlags_DefaultOpen;

    if (!current.is_parent())flags |= ImGuiTreeNodeFlags_Leaf;


    std::string name = make_entity_name(current);

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
            for (const auto& child : current.get_children()) {
                render_entity(working_scene->uuid_to_entity(child), has_selected);
            }
        }

        ImGui::TreePop();
    }
}



void EScene::render_editorview(float dt) {

    ImGui::Begin("Editor View");

    ImVec2 size =ImGui::GetContentRegionAvail();
    ImVec2 pos = ImGui::GetCursorScreenPos();

    auto& view_camera = viewer.get_component<CameraComp>();

    setup_imguizmo(pos, size);


    glm::vec2 new_size(size.x, size.y);
    if (pickerview.last_scale != new_size) {
        rescale_framebuffer(pickerview, size.x, size.y);
    }

    draw_to_camera(manager->render_data,{size.x, size.y}, viewer, working_scene->registry, &working_scene->s_render_data,true, view_camera.framebuffer);
    render_hitboxes();
    render_pickerview();
    
    ImRect view_rect(pos, ImVec2(pos.x + size.x, pos.y + size.y));

    ImGui::ItemAdd(view_rect, 0);
    ImGui::GetWindowDrawList()->AddImage(
        (void*)(intptr_t)view_camera.framebuffer.texture,
        view_rect.Min, view_rect.Max,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);


    //render_grid(pos, size);
    render_gizmo(pos, size);

    auto& editor_viewer_state = get_editorviewer_state();

    switch (editor_viewer_state) {
        // ====================================================================== 
        case EditorViewer::State::Natual: {

            glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;

            auto& lua = viewer.get_component<LuaActionComp>();
            lua.bind_field("first", true);

            if (ImGui::IsWindowHovered()  && is_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) { editor_viewer_state = EditorViewer::State::FPS;  break;}
            if (ImGuizmo::IsUsing()) { editor_viewer_state = EditorViewer::State::UsingGizmo; break; }



            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.7f, 0.7f));

            glm::vec4 anticolor = glm::vec4(1.0f) - working_scene->s_render_data.clear_color;
            anticolor *= 255.0f;
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(anticolor.x, anticolor.y, anticolor.z, 255));
            ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(anticolor.x, anticolor.y, anticolor.z, 255));
            ImGui::SetCursorPos({20.0f,10.0f});

            bool radio = false;

            if (ImGui::RadioButton( ICON_MD_CONTROL_CAMERA "", guizmo_operation == ImGuizmo::OPERATION::TRANSLATE) || is_key_clicked(GLFW_KEY_T))
            {guizmo_operation = ImGuizmo::OPERATION::TRANSLATE; radio = true; }
            ImGui::SameLine(0, 3.0f);
            if (ImGui::RadioButton( ICON_MD_OPEN_IN_NEW "", guizmo_operation == ImGuizmo::OPERATION::SCALE)|| is_key_clicked(GLFW_KEY_S))
            {guizmo_operation = ImGuizmo::OPERATION::SCALE; radio = true; }
            ImGui::SameLine(0, 3.0f);
            if (ImGui::RadioButton( ICON_MD_LOOP "", guizmo_operation == ImGuizmo::OPERATION::ROTATE)|| is_key_clicked(GLFW_KEY_R))
            {guizmo_operation = ImGuizmo::OPERATION::ROTATE; radio = true; }

            ImGui::SameLine(0.0, 7.5f);
            ImGui::Text("|");
            ImGui::SameLine(0.0, 7.5f);
            if (ImGui::RadioButton( ICON_MD_PUBLIC "", guizmo_mode == ImGuizmo::MODE::WORLD))
            {guizmo_mode = ImGuizmo::MODE::WORLD; radio = true; }

            ImGui::SameLine(0.0, 3.0f);
            if (ImGui::RadioButton( ICON_MD_LOCATION_PIN "", guizmo_mode == ImGuizmo::MODE::LOCAL))
            {guizmo_mode = ImGuizmo::MODE::LOCAL; radio = true; }
            

            gizmo_snap = is_key_pressed(GLFW_KEY_LEFT_CONTROL);
            gizmo_fine = is_key_pressed(GLFW_KEY_LEFT_SHIFT);

            ImGui::PopStyleVar(1);
            ImGui::PopStyleColor(2);

            if (ImGui::IsMouseHoveringRect(view_rect.Min, view_rect.Max) && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !radio)
            {pick_entity(view_rect);}

        }break;



        // ====================================================================== 
        case EditorViewer::State::UsingGizmo: {
            if (!is_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {editor_viewer_state = EditorViewer::State::Natual; break;}
            ImGui::GetIO().WantCaptureMouse = false;

            gizmo_fine = is_key_pressed(GLFW_KEY_LEFT_SHIFT);
            gizmo_snap = is_key_pressed(GLFW_KEY_LEFT_CONTROL);
        }break;


        // ====================================================================== 
        case EditorViewer::State::FPS: {
            if(!is_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {editor_viewer_state = EditorViewer::State::Natual; break;}

            ImGui::GetIO().WantCaptureMouse = true;
            glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
            auto& lua = viewer.get_component<LuaActionComp>();
            lua.get_last().on_update(dt);
        }break;


        // ====================================================================== 
        default: DU_ASSERT(true, "Unknown editor_viewer_state {}", (int)editor_viewer_state);
    }


    ImGui::End();
}


void EScene::render_pickerview() {
    ImVec2 size = ImGui::GetContentRegionAvail();

    glViewport(0,0, size.x, size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, pickerview.handler);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto camera = viewer.get_component<CameraComp>();
    auto objects = working_scene->registry.view<TransformComp, ModelComp>();

    glUseProgram(picker_shader);

    for (const auto& [e, t, m] : objects.each()) {
        int r = ((uint32_t)e & 0x000000FF) >>  0;
        int g = ((uint32_t)e & 0x0000FF00) >>  8;
        int b = ((uint32_t)e & 0x00FF0000) >> 16;
        set_shader_v3(picker_shader, "id_color", {r/255.0f, g/255.0f, b/255.0f});
        glm::mat4 model = t.get_model();
        set_shader_m4(picker_shader, "model", model);

        glBindVertexArray(m.model.VAO);
        if (m.model.nindices != 0) {
            glDrawElements(GL_TRIANGLES, m.model.nindices, GL_UNSIGNED_INT, 0);
            continue;
        }
        glDrawArrays(GL_TRIANGLES, 0, m.model.nvertices);
    }

    auto hitboxes = working_scene->registry.view<TransformComp>(entt::exclude<ModelComp>);
    const Model& cube = get_model("cube");
    for (const auto& [e, t] : hitboxes.each()) {
        int r = ((uint32_t)e & 0x000000FF) >>  0;
        int g = ((uint32_t)e & 0x0000FF00) >>  8;
        int b = ((uint32_t)e & 0x00FF0000) >> 16;
        set_shader_v3(picker_shader, "id_color", {r/255.0f, g/255.0f, b/255.0f});
        glm::mat4 model = t.get_model();
        set_shader_m4(picker_shader, "model", model);

        glBindVertexArray(cube.VAO);
        if (cube.nindices != 0) {
            glDrawElements(GL_TRIANGLES, cube.nindices, GL_UNSIGNED_INT, 0);
            continue;
        }
        glDrawArrays(GL_TRIANGLES, 0, cube.nvertices);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EScene::render_hitboxes() {
    auto& camera = viewer.get_component<CameraComp>();
    glBindFramebuffer(GL_FRAMEBUFFER, camera.framebuffer);
    glEnable(GL_DEPTH_TEST);

    auto hitboxes = working_scene->registry.view<TransformComp>(entt::exclude<ModelComp>);
    const Model& cube = get_model("cube");
    Shader& shader = get_shader("basic.glsl");

    entt::entity selected_entt = (selected) ? working_scene->uuid_to_entt(selected) : entt::null;

    glUseProgram(shader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (const auto& [e, t] : hitboxes.each()) {
        if (e == selected_entt) continue;
        set_shader_v3(shader, "material.ambient", {0.5f,0.5f,0.5f});
        glm::mat4 model = t.get_model();
        set_shader_m4(shader, "model", model);

        glBindVertexArray(cube.VAO);
        if (cube.nindices != 0) {
            glDrawElements(GL_TRIANGLES, cube.nindices, GL_UNSIGNED_INT, 0);
            continue;
        }
        glDrawArrays(GL_TRIANGLES, 0, cube.nvertices);
    }
    
    if(selected) {

        Entity tmp = working_scene->uuid_to_entity(selected);
        set_shader_v3(shader, "material.ambient", {0.0f,0.0f,0.0f});
        if (!tmp.has_component<TransformComp>()) goto end;
        glm::mat4 model = tmp.get_component<TransformComp>().get_model();
        set_shader_m4(shader, "model", model);

        glBindVertexArray(cube.VAO);
        if (cube.nindices != 0) {
            glDrawElements(GL_TRIANGLES, cube.nindices, GL_UNSIGNED_INT, 0);
            goto end;
        }
        glDrawArrays(GL_TRIANGLES, 0, cube.nvertices);
    }

end:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

entt::entity EScene::entity_from_view(ImVec2 pos, ImVec2 size) {

    if (pos.x >= size.x || pos.y >= size.y || pos.x < 0 || pos.y < 0)
        return entt::null;
    unsigned char pixel[4]; 
                            
    glGetTextureSubImage(
        pickerview.texture,
        0,        
        pos.x, pos.y, 0,  
        1, 1, 1, 
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        sizeof(pixel),
        pixel
    );

    int picked = 
        pixel[0] + 
        pixel[1] * 256 +
        pixel[2] * 256*256;
    if (picked == 0x00FFFFFF) {
        return entt::null;
    }

    return entt::entity(picked);
}

void EScene::pick_entity(ImRect view_rect) {

    ImVec2 size = view_rect.GetSize();
    ImVec2 mouse = ImGui::GetMousePos();

    ImVec2 local = ImVec2(mouse.x - view_rect.Min.x, mouse.y - view_rect.Min.y);
    float w = size.x;
    float h = size.y;

    if (w <= 0.0f || h <= 0.0f) return;

    float u = local.x / w;
    float v = local.y / h;
    v = 1.0f - v;



    ImGuiIO& io = ImGui::GetIO();
    float fb_scale_x = io.DisplayFramebufferScale.x; 
    float fb_scale_y = io.DisplayFramebufferScale.y;

    int tex_w = size.x * fb_scale_x;
    int tex_h = size.y * fb_scale_y;

    int tex_x = (int)floorf(u * (float)tex_w);
    int tex_y = (int)floorf(v * (float)tex_h);

    tex_x = std::max(0, std::min(tex_x, tex_w - 1));
    tex_y = std::max(0, std::min(tex_y, tex_h - 1));

    entt::entity picked = entity_from_view(ImVec2(tex_x, tex_y), size);

    if (picked != entt::null){
        Entity e{working_scene, picked};
        selected = e.uuid();
    }
    else { selected = 0; }

}

void EScene::render_prj_settings() {
    ImGui::Begin("Project Settings", nullptr,ImGuiWindowFlags_NoDocking);
    ProjectData& data = manager->project_data;

    if (ImGui::TreeNodeEx("Globals", ImGuiTreeNodeFlags_DefaultOpen)) {
        render_str_select("Startup Scene: ", data.startup_scene, resource_lists.scenes);
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("Layers")) {
        auto& layers = manager->render_data.layers_atrb;
        auto size = ImGui::GetWindowSize();
        ImGui::BeginChild("##LayersConfig",{ size.x * 0.95f, size.y * 0.6f}, ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
            
        int i = 0;
        for(auto& layer : layers) {
            ImGui::PushID(578 + i);
            ImGui::BulletText("Layer Number%s%d", (i >= 10) ? " " : "  ",i);
            ImGui::SameLine(0, size.x *0.05f);
            ImGui::Checkbox("Wireframe", &layer.wireframe);
            ImGui::SameLine(0, size.x *0.04f);
            ImGui::Checkbox("Depth Test", &layer.depth);
            ImGui::Separator();
            i++;
            ImGui::PopID();
        }

        ImGui::EndChild();
        ImGui::TreePop();
    }
;

    //ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.9f);
    if (ImGui::Button("Apply")) {
        std::string prj_path = std::format("{}/{}.prj", data.root_path.string(), data.name).c_str();
        write_project_file(prj_path.c_str(), data, manager->render_data.layers_atrb.data());
        show_project_settings = false;
    }
    ImGui::SameLine(0, 10.0f);
    if (ImGui::Button("Close"))
        show_project_settings = false;
    ImGui::End();
}

void SceneSetting::render_if_on() {
    if (!(*this)) return;
    ImGui::Begin("Scene Settings", nullptr, ImGuiWindowFlags_NoDocking);
    ImGui::Text("Name: %s", ptr->name.c_str());

    if (ImGui::TreeNodeEx("Rendering", ImGuiTreeNodeFlags_DefaultOpen)) {
        sameline_color("Clear Color: ", ptr->s_render_data.clear_color);
        sameline_color("Ambient Light Color: ", ptr->s_render_data.ambient);
        sameline_float("Ambient Light Strength: ", &ptr->s_render_data.ambient_strength, 0.0f, 1.0f);


        uuid_select<TransformComp, CameraComp>("Main Camera: ", ptr, ptr->main_camera);
        ImGui::TreePop();
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.9f);
    if (ImGui::Button("Close"))
        off();

    ImGui::End();
}


static std::string scene_name = "";
static bool set_current = true;

void EScene::render_create_scene() {
    ImGui::Begin("Create a new scene");
    ImGui::Indent();

       if ((ImGui::IsWindowFocused() || !ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) && !ImGui::IsAnyItemActive())
           ImGui::SetKeyboardFocusHere();
        sameline_text("Scene Name", &scene_name);
        bool exists = (scene_name.empty()) ? false : manager->get_scenes().contains(scene_name);
        if(exists) {
            ImGui::Dummy({0, 10.0f});
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0,0,255));
            ImGui::Text("Error: Scene \"{}\" already exists.");
            ImGui::PopStyleColor();
        }


        if (!exists && !scene_name.empty()) {
            ImGui::Dummy({0, 10.0f});
            if(working_scene)
                sameline_checkbox("Make Current", &set_current);
        }

        static bool set_startup = false;
        if (!exists && !scene_name.empty() && manager->project_data.startup_scene != scene_name) {
            ImGui::Dummy({0, 10.0f});
            sameline_checkbox("Set As Startup", &set_startup);

        }

        //ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.9f);
        if (working_scene) {
            if(ImGui::Button("Cancel")) {
                ImGui::Unindent();
                ImGui::End();
                scene_name = "";
                set_current = true;
                creating_scene = false;
                return;
            }
            ImGui::SameLine(0, 10.0f);
        }

        if (!exists && !scene_name.empty()) {
            if(ImGui::Button("Create") || is_key_pressed(GLFW_KEY_ENTER)) {
                Scene* tmp = create_scene(scene_name.c_str());
                if(set_startup)
                    manager->project_data.startup_scene = scene_name;
                if (set_current)
                    working_scene = tmp;
                set_current = true;
                scene_name = "";

                creating_scene = false;
                resource_lists = ResourceLists();
                resource_lists.init(manager);
            }
        }

    ImGui::Unindent();
    ImGui::End();
}

void EScene::render_resources() {
    ImGui::Begin("Resources");
    const ResourceLists& lists = resource_lists;

    if (ImGui::TreeNodeEx("Scenes", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (const auto& scene : lists.scenes) {
            ImGui::BulletText("%s", scene.c_str());
            if (working_scene->name == scene) {
                if (!show_scene_settings) {
                    ImGui::SameLine(0, 5.0f);
                    std::string title = std::format("{}##SceneSettings{}", ICON_MD_SETTINGS,scene);
                    if(ImGui::Button(title.c_str())) {
                        show_scene_settings.on(working_scene);
                    }
                }
                continue;
            };

            ImGui::SameLine(0, 15.0f);
            if (ImGui::Button("Set Current")) {
                working_scene = manager->get_scene(scene.c_str());
                show_scene_settings.off();
                selected = 0;
                if (!working_scene->file_path.empty() && working_scene->uuids.empty())
                    working_scene->add_from_file(working_scene->file_path.c_str());
            }


        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("Resources", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::TreeNode("Shaders")) {
            for (const auto& shader : lists.shaders) {
                ImGui::BulletText("%s", shader.c_str());
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Textures")) {
            for (const auto& texture : lists.textures) {
                ImGui::BulletText("%s", texture.c_str());
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Models")) {
            for (const auto& model : lists.models) {
                ImGui::BulletText("%s", model.c_str());
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Scripts")) {
            for (const auto& script : lists.scripts) {
                ImGui::BulletText("%s", script.c_str());
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

void EScene::setup_imguizmo(ImVec2 pos, ImVec2 size) {

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::AllowAxisFlip(false);
    ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);
    ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);
}

void EScene::render_grid(ImVec2 pos, ImVec2 size) {
    TransformComp& view_trans = viewer.get_component<TransformComp>();
    CameraComp& view_cam = viewer.get_component<CameraComp>();
    glm::mat4 projection = view_cam.get_projection({size.x, size.y});
    glm::mat4 view = view_cam.get_view(view_trans.position());

    glm::mat4 matrix(1.0f);

    ImGuizmo::DrawGrid(glm::value_ptr(view), glm::value_ptr(projection), glm::value_ptr(matrix), 200.0f);
}

void EScene::render_gizmo(ImVec2 pos, ImVec2 size) {

    if (!selected) return;

    TransformComp& view_trans = viewer.get_component<TransformComp>();
    CameraComp& view_cam = viewer.get_component<CameraComp>();

    glm::mat4 projection = view_cam.get_projection({size.x, size.y});
    glm::mat4 view = view_cam.get_view(view_trans.position());

    Entity e_selected = working_scene->uuid_to_entity(selected);
    if (!e_selected.has_component<TransformComp>()) return;

    TransformComp& selected_trans = e_selected.get_component<TransformComp>();


    glm::mat4 matrix = selected_trans.get_model();
    glm::mat4 delta(1.0f);



    float scale_snap[] = {0.5f, 0.5f, 0.5f};
    float rotate_snap = 36.0f;

    if(gizmo_fine) {
        rotate_snap /= 4.0f;
        for (int i = 0; i < 3; i++)
            scale_snap[i] /= 4.0f;
    }

    float* snap_ptr = (guizmo_operation == ImGuizmo::OPERATION::ROTATE) ? &rotate_snap : scale_snap;


    ImGuiIO& io = ImGui::GetIO();
    bool ctrl_backup = io.KeyCtrl;
    io.KeyCtrl = true;     


    ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), guizmo_operation, guizmo_mode,glm::value_ptr(matrix), glm::value_ptr(delta), (gizmo_snap) ? snap_ptr : nullptr);

    if (ImGuizmo::IsUsing())
        selected_trans.apply_mat(delta);

    io.KeyCtrl = ctrl_backup;
}

