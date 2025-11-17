#include "components/modelcomp.h"
#include "helper.h"
#include "imgui_internal.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>

#include <tinyfiledialogs.h>


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
    io.FontGlobalScale = 1.5f;
}


void open_working_file(SceneManager* manager, RTScene* working_scene, EScene* editor){
    char* path = tinyfd_openFileDialog("Open a Scene file", nullptr, 0, nullptr, nullptr,0);
    if(!path) return;
    manager->clear_scene(working_scene);
    working_scene->add_from_file(path);
    editor->save_path = path;
}


void save_working_file(SceneManager* manager, EScene* editor) {
    if (editor->save_path == "UNSET") {
        char* path= tinyfd_saveFileDialog("Save Current Scene", nullptr, 0, nullptr, nullptr);
        if (!path) return;
        editor->save_path = path;
    }
    manager->write_scene_to_file(editor->save_path.c_str(), editor->working_scene); 
    DU_TRACE("Saving to [{}]", editor->save_path);
}


void saveas_working_file(SceneManager* manager, EScene* editor) {
    char* path= tinyfd_saveFileDialog("Save Current Scene As", nullptr, 0, nullptr, nullptr);
    if (!path) return;
    manager->write_scene_to_file(path, editor->working_scene); 
    DU_TRACE("Saving to [{}]", path);
}

size_t counter = 0;

EditorState EScene::update_imgui(float dt) {

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    EditorState state = EditorState::Normal;

    counter++;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    int _open[] = {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_O};
    int _save[] = {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_S};
    int _saveas[] = {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_S};

    if (check_key_combo(_open, 2) && counter > 10){
        open_working_file(manager, working_scene, this);
        counter = 0;
    }
    else if (check_key_combo(_saveas, 3) && counter > 10){
        saveas_working_file(manager, this);
        counter = 0;
    }
    else if (check_key_combo(_save, 2) && counter > 10){
        save_working_file(manager, this);
        counter = 0;
    }
    else if (is_key_pressed(GLFW_KEY_F5) && counter > 10) {
        state = EditorState::Preview;
        counter = 0;
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl + O")) {
                open_working_file(manager, working_scene, this);
            }
            if (ImGui::MenuItem("Save", "Ctrl + S")) {
                save_working_file(manager,this);
            }
            if (ImGui::MenuItem("Save As", "Ctrl + Shift + S")) {
                saveas_working_file(manager,this);
            }

            if (ImGui::MenuItem("Project Settings")) {
                show_project_settings = true;
            }


            if (ImGui::MenuItem("Quit")) close = true;
            ImGui::EndMenu();
        }

        ImGui::Spacing();
        if (ImGui::ArrowButton("Play", ImGuiDir::ImGuiDir_Right)) {
            state = EditorState::Preview;
        }
        


        ImGui::EndMainMenuBar();
    }

    // Create a dockspace
    ImGui::DockSpaceOverViewport();

    bool has_selected = false;
    render_entities(&has_selected);
    render_overview(has_selected);

    render_editorview(dt);
    if (show_project_settings)
        render_psettings();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return state;
}

void EScene::end_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EScene::render_entities(bool *has_selected) {
    ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {

        if (ImGui::BeginMenu("New")) {
            if( ImGui::MenuItem("Empty Entity") ) {
                auto e = working_scene->create_entity();
                if (selected) {
                    e.make_child_of(selected);
                }
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
    

    for (auto [_, entity] : working_scene->uuids) {
        Entity tmp = {working_scene, entity};
        if (tmp.is_child())// || tmp.has_component<EditorViewer>())
            continue;
        render_entity(tmp, has_selected, true);
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
            for (auto child : current.get_children()) {
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

    static bool looking = false;
    if ((ImGui::IsWindowHovered() || looking) && is_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {

        glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        auto& lua = viewer.get_component<LuaActionComp>();
        lua.get_last().on_update(dt);

        looking = true;
    }
    else {
        looking = false;
        glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        auto& lua = viewer.get_component<LuaActionComp>();
        lua.bind_field("first", true);
    }

    if (glm::vec2(size.x, size.y) != glm::vec2(editorview.last_scale.x, editorview.last_scale.y)) {
        rescale_framebuffer(editorview, size.x, size.y);
        rescale_framebuffer(pickerview, size.x, size.y);
    }

    glViewport(0,0, size.x, size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, editorview.handler);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    opengl_renderer(manager->render_data,{size.x, size.y}, viewer, working_scene->registry, true);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    render_pickerview();

    ImGui::InvisibleButton("editor_viewport", size);

    ImGui::GetWindowDrawList()->AddImage(
        (void*)(intptr_t)editorview.texture,
        pos,
        ImVec2(pos.x + size.x, pos.y + size.y),  
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    if (ImGui::IsWindowHovered() && 
        ImGui::IsMouseReleased(ImGuiMouseButton_Left) && 
        !looking) {

        ImVec2 mouse = ImGui::GetMousePos();
        ImVec2 item_min = ImGui::GetItemRectMin();
        ImVec2 item_max = ImGui::GetItemRectMax();

        ImVec2 local = ImVec2(mouse.x - item_min.x, mouse.y - item_min.y);
        float w = item_max.x - item_min.x;
        float h = item_max.y - item_min.y;

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

    ImGui::End();
}


void EScene::render_pickerview() {
    ImVec2 size = ImGui::GetContentRegionAvail();

    glViewport(0,0, size.x, size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, pickerview.handler);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto trans = viewer.get_component<TransformComp>();
    auto camera = viewer.get_component<CameraComp>();
    auto objects = working_scene->registry.view<TransformComp, ModelComp>();

    glUseProgram(picker_shader);

    for (auto [e, t, m] : objects.each()) {
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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

entt::entity EScene::entity_from_view(ImVec2 pos, ImVec2 size) {

    if (pos.x >= size.x || pos.y >= size.y || pos.x < 0 || pos.y < 0)
        return entt::null;
    unsigned char pixel[4]; // RGBA
                            //
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
    // Convert the color back to an integer ID
    int picked = 
        pixel[0] + 
        pixel[1] * 256 +
        pixel[2] * 256*256;
    if (picked == 0x00FFFFFF) {
        return entt::null;
    }

    return entt::entity(picked);
}

void EScene::render_psettings() {
    ImGui::Begin("Project Settings");

    if (ImGui::CollapsingHeader("Global Variables")) {
        ImGui::Indent();

        ImGui::Text("Main camera: "); ImGui::SameLine();
        ImGui::SetCursorPosX(get_centered_pos("Main Camera: "));
        UUID current = working_scene->main_camera;
        std::string sample = "Not Set";
        if (current) {
            Entity tmp = working_scene->uuid_to_entity(current);
            sample = make_entity_name(tmp);
        }

        auto view = working_scene->registry.view<CameraComp, TransformComp>();

        if( ImGui::BeginCombo("##", sample.c_str()) ) {
            for (auto& e : view) {
                Entity tmp = {working_scene, e};
                bool is_selected = (current) ? e == working_scene->uuid_to_entt(current) : false;
                if (ImGui::Selectable(make_entity_name(tmp).c_str(), is_selected)) {
                    current = working_scene->entt_to_uuid(e);
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (current)
            working_scene->main_camera = current;


    }

    if (ImGui::CollapsingHeader("Layers")) {

    }

    if (ImGui::Button("Close"))
        show_project_settings = false;
    ImGui::End();
}
