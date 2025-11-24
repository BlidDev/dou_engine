#include "greeter.h"
#include "GLFW/glfw3.h"
#include "helper.h"
#include <format>
#include <tinyfiledialogs.h>

#include <filesystem>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

extern ImGuiContext* GImGui;

Greeter::Greeter() : Scene("Greeter"){
    state = GreeterState::GreeterNormal;
    tmp_data = ProjectData();
    tmp_data.root_path = std::filesystem::current_path() / "projects";
}

void Greeter::on_create() {

    if (GImGui != nullptr) return;
    initialize_imgui(manager);
    detect_projects();
}

void Greeter::on_update(float dt) {
    glClearColor(0.5f,0.5f,0.5f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    switch(state) {
        case GreeterState::GreeterNormal: 
            {
                ImGui::Begin("Greeter");
                ImGui::Dummy({0, 40});
                ImGui::Indent();
                    ImGui::Text("Welcome To Dou Engine! Would You Like To...");
                    ImGui::Dummy({0, 10});

                    if(ImGui::Button("Create A New Project")) 
                        state = GreeterState::GreeterCreation;
                    if(ImGui::Button("Open An Existing Project")) {
                        const char* filter[] = {"*.prj"};
                        const char* path = tinyfd_openFileDialog("Open A Project File", 
                                (std::filesystem::current_path() / "projects").c_str(),
                                1, filter, nullptr, false);
                        open_project(path, true);
                    }

                    render_existing_projects();
                
                ImGui::Unindent();
                ImGui::End();
            }
            ;break;
        case GreeterState::GreeterCreation: create_project();break;

    };

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}

void Greeter::on_end() {
}

bool Greeter::should_close() {
   return glfwWindowShouldClose(manager->main_window);
}

void Greeter::create_project() {
    ImGui::Begin("Create A New Project");
    ImGui::Indent();
       ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.1f);
       if ((ImGui::IsWindowFocused() || !ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) && !ImGui::IsAnyItemActive())
           ImGui::SetKeyboardFocusHere();
       sameline_text("Project Name", &tmp_data.name);
       if (tmp_data.name.empty()) {
            ImGui::Unindent();
            ImGui::End();
            return;
       }

       ImGui::Dummy({0, 10.0f});
       std::filesystem::path current(tmp_data.root_path / tmp_data.name);
       ImGui::Text("Current Location: %s", current.c_str());
       bool exists = std::filesystem::is_directory(current);
       ImGui::SameLine(0, 10.0f);
       if (ImGui::Button("Browse...")) {
            char* path = tinyfd_selectFolderDialog("Select Folder For Project", tmp_data.root_path.c_str());
            if(path) {
                tmp_data.root_path = path;
            }
       }

       if(exists) {
           ImGui::Dummy({0, 10.0f});
           ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0,0,255));
           ImGui::Text("Error: Project path already exists! Please change the path to a non existing one.");
           ImGui::PopStyleColor();
       }
       ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.9f);
       if (!exists) {
           if (ImGui::Button("Create") || is_key_pressed(GLFW_KEY_ENTER)) {
               std::filesystem::copy("res/essentials", current, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
               auto last = tmp_data.root_path;
               tmp_data.root_path = current.lexically_relative(std::filesystem::current_path());
               tmp_data.scene_paths.push_back("scenes");
               tmp_data.shader_paths.push_back("shaders");
               tmp_data.texture_paths.push_back("textures");
               tmp_data.model_paths.push_back("models");

               std::string prj_path = std::format("{}/{}.prj", current.string(), tmp_data.name).c_str();
               write_project_file(prj_path.c_str(), tmp_data);
               tmp_data.root_path = last;
               open_project(prj_path.c_str(), true);
           }
           ImGui::SameLine(0, 10.0f);
       }
       if (ImGui::Button("Back")) {
           tmp_data = ProjectData();
           state = GreeterState::GreeterNormal;
       }

    ImGui::Unindent();
    ImGui::End();
}


void Greeter::open_project(const char* path, bool add_paths) {
    if (!path) return;

    const ProjectData& data = read_project_file(path, manager, add_paths, false);
    if (!data.startup_scene.empty()) {
        DU_DEBUG_TRACE("Selected scene: {}", data.startup_scene);
        editor->working_scene = manager->get_scene(data.startup_scene.c_str());
    }

    manager->set_current("EDITOREditor");
}

namespace fs = std::filesystem;

void Greeter::detect_projects() {
    for(const auto& entry : fs::directory_iterator("projects")) {
        if (!entry.is_directory()) continue;
        std::string name = entry.path().filename().string();
        fs::path path = std::format("projects/{}/{}.prj", name, name);
        if (!fs::exists(path)) continue;
        detected_projects.push_back(path);
        DU_DEBUG_TRACE("Detected: {}", path.c_str());
    }
}


void Greeter::render_existing_projects() {
    if(detected_projects.empty()) return;

    ImGui::Dummy({0, 25.0f});
    ImGui::Separator();
    ImGui::Dummy({0, 25.0f});
    ImGui::BeginChild("Detected Projects", 
            ImVec2(ImGui::GetContentRegionAvail().x * 0.95f, 
                   ImGui::GetWindowHeight() * 0.3f)
            , ImGuiChildFlags_Borders, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        ImGui::Text("Detected Projects");
        ImGui::EndMenuBar();
    }

    for (const auto& path : detected_projects) {
        ImGui::Text("%s", path.c_str());
        ImGui::SameLine();
        float width = ImGui::GetWindowWidth();
        ImGui::SetCursorPosX((width * 0.93)- ImGui::CalcTextSize("Open").x);
        if (ImGui::Button(std::format("Open##{}", path).c_str())) {
            open_project(path.c_str(), true);
        }
    }

    ImGui::EndChild();
}
