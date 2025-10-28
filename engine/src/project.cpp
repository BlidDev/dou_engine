#include <espch.h>
#include <sys/stat.h>
#include <filesystem>
#include "project.h"


#define TRY_NODE(node,name, type, lhs) if(node[name]){ lhs = node[name].as<type>();}

namespace engine {

    void read_layers(YAML::Node& node, SceneManager* manager);
    void read_paths(YAML::Node& node, SceneManager* manager);

    int read_project_file(const char* path, SceneManager* manager, std::string* name) {
        std::ifstream file(path);
        EG_ASSERT(file.fail(), "Project file path \"{}\" does not exist", path);
        std::stringstream str_stream;
        str_stream<<file.rdbuf();

        YAML::Node data = YAML::Load(str_stream.str());

        EG_ASSERT(!data["Project Name"], "Project name not provided in \"{}\"", path);
        *name = data["Project Name"].as<std::string>();

        YAML::Node current = data["Current Scene"];
        EG_ASSERT(!current, "Project does not provide current scene name");

        read_layers(data, manager);
        read_paths(data, manager);

        manager->set_current(current.as<std::string>().c_str());
        return 0;

    }

    void read_layers(YAML::Node& node, SceneManager* manager) {
        auto layers = node["Layers"];
        if (!layers) return;

        for(int i = 0; i < MAX_RENDER_LAYERS; i++) {
            std::string name = std::format("Layer {}", i);
            auto layer = layers[name];
            if (!layer) continue;
            TRY_NODE(layer, "Depth Test", bool, manager->get_layer_atrb(i)->depth);
            TRY_NODE(layer, "Wireframe Mode", bool, manager->get_layer_atrb(i)->wireframe);
            bool tmp = false;
            TRY_NODE(layer, "Is Framebuffer", bool, tmp);
            //if (tmp) set_layer_to_framebuffer(manager, i);
        }
    }

    void read_paths(YAML::Node& node, SceneManager* manager) {
        namespace fs = std::filesystem; 

        auto paths = node["Paths"];
        if (!paths) return;

        auto shaders = paths["Shaders"];
        if (shaders) {
            for (auto path : shaders) {
                std::string p = path.as<std::string>();
                for (const auto & entry : fs::directory_iterator(p)) {
                    fs::path fs(entry);
                    if (fs.extension() != ".glsl") continue;
                    std::string final = std::format("{}/{}", p,fs.filename().string());
                    manager->register_shader(final.c_str());
                }
            }
        }

        auto textures = paths["Textures"];
        if (textures) {
            for (auto path : textures) {
                std::string p = path.as<std::string>();
                for (const auto & entry : fs::directory_iterator(p)) {
                    fs::path fs(entry);
                    if (fs.extension() != ".png" && fs.extension() != "jpg") continue;
                    std::string final = std::format("{}/{}", p,fs.filename().string());
                    manager->register_texture(final.c_str());
                }
            }
        }

        auto models = paths["Models"];
        if (models) {
            for (auto path : models) {
                std::string p = path.as<std::string>();
                for (const auto & entry : fs::directory_iterator(p)) {
                    fs::path fs(entry);
                    if (fs.extension() != ".sff") continue;
                    std::string final = std::format("{}/{}", p,fs.filename().string());
                    std::string model_name = "unnamed";

                    Model model = model_from_file(final.c_str(), &model_name);
                    EG_ASSERT(model_name == "unnamed", "Model {} wasn't given any name");
                    manager->register_model(model_name.c_str(), model);
                }

            }
        }

    }
}
