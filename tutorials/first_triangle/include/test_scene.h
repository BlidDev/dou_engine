// test_scene.h

#include <engine.h>

class TestScene : public engine::Scene {
public:
    TestScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 

private:
    bool close; 
    engine::Entity player;
};
