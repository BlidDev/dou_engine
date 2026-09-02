#pragma once
#include <engine.h>


class BScene : public engine::Scene {
public:
    BScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 

    void render_quad(glm::vec3 position, glm::vec3 color);
private:
    bool close;
    engine::Entity player;

#define MAX_PER 100
    unsigned int VAO, VBO;
    engine::Shader shader, otherquad;
    std::array<glm::vec3, MAX_PER> transes;
};
