#pragma once
#include "log.h"
#include "scene.h"
#include "component.h"
#include "systems.h"
#include "entity.h"
#include "ops.hpp"

namespace engine {
    extern int on_start(engine::SceneManager* manager);
    extern int on_end();
}
