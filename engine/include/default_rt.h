#pragma once
#include "scene.h"
#include "entity.h"


namespace engine {

    class DefaultRT : public engine::Scene {
    public:
        DefaultRT();

        void on_create();
        void on_update(float dt);
        void on_end();
        bool should_close(); 

    private:
        bool close;
        Entity viewer;
    };
}
