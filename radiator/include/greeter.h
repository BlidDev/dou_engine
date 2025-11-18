#pragma once

#include "editors.h"
#include <epch.h>

using namespace engine;

enum GreeterState {
    GreeterNormal,
    GreeterCreation
};

class Greeter : public Scene {
public:
    Greeter();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 


    void create_project();
    void open_project(const char* path, bool add_paths);

    GreeterState state;
    EScene* editor;
    ProjectData tmp_data;
};
