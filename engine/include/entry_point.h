#pragma once
#include "manager.h"

namespace engine {
    extern int on_start(engine::SceneManager* manager);
    extern int on_end(engine::SceneManager* manager);

    void set_frame_cap(int frame_cap);
}
