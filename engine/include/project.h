#pragma once

// shaders
// textures
// current
// models
// layers

namespace fs = std::filesystem;


namespace engine {
    class SceneManager;
    class Scene;
    
    struct ProjectData {
        std::string name;
        std::filesystem::path root_path;
        std::vector<fs::path> scene_paths;
        std::vector<fs::path> shader_paths;
        std::vector<fs::path> texture_paths;
        std::vector<fs::path> model_paths;

        ProjectData(std::string name = "");
    };

    int read_project_file(const char* path, SceneManager* manager, bool set_current = false);


    extern Scene* create_runtime_scene();
}
