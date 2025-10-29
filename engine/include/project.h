#pragma once

// shaders
// textures
// current
// models
// layers

#include "manager.h"
namespace engine {

    int read_project_file(const char* path, SceneManager* manager, std::string* name, bool set_current = false);
}
