#pragma once
#include "scene.h"
#include "component.h"
#include "systems.h"
#include "entity.h"

namespace engine {
    extern int on_start(engine::SceneManager* manager);
    extern int on_end();
}
