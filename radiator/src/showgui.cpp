#include "editors.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
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
    // io.FontGlobalScale = 1.5f;
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
    EG_TRACE("Saving to [{}]", editor->save_path);
}


void saveas_working_file(SceneManager* manager, EScene* editor) {
    char* path= tinyfd_saveFileDialog("Save Current Scene As", nullptr, 0, nullptr, nullptr);
    if (!path) return;
    manager->write_scene_to_file(path, editor->working_scene); 
    EG_TRACE("Saving to [{}]", path);
}

size_t counter = 0;

EditorState EScene::update_imgui(float dt) {
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

    // Rendering
    ImGui::Render();
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return state;
}

void EScene::end_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EScene::render_entities(bool *has_selected) {
    ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_HorizontalScrollbar);

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

    if (glm::vec2(size.x, size.y) != glm::vec2(editorview.last_scale.x, editorview.last_scale.y))
        rescale_framebuffer(editorview, size.x, size.y);

    glViewport(0,0, size.x, size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, editorview.handler);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    opengl_renderer(manager->render_data,{size.x, size.y}, viewer, working_scene->registry, true);
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

