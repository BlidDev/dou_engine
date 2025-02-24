#pragma once
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "component.h"
#include "entity.h"
#include "models.h"
#include "systems.h"

namespace engine {
    extern int on_start(engine::SceneManager* manager);
    extern int on_end();
}
