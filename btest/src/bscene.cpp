
#include "bscene.h"
#include "renderer.h"

using namespace engine;

void render_batched(const Shader& shader, const unsigned int& VAO, size_t num, size_t v_num, size_t i_num);

BScene::BScene() : Scene("thing") {
    close = false;
}

void BScene::on_create() {
    VAO = 0; VBO = 0;

    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            glm::vec3 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translation.z = 0.0f;
            transes[index++] = translation;
        }
    }

    std::array<glm::vec3, MAX_PER> colors;
    for (int i = 0; i < colors.size(); i++)
        colors[i] = (i%2 == 0) ? glm::vec3(1.0f) : glm::vec3{0.5f,0.5f,0.5f};

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_PER, &transes[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int cVBO = 0;
    glGenBuffers(1, &cVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_PER, &colors[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    VAO = get_mesh("quad_tex").VAO;
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, cVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(4, 1); // tell OpenGL this is an instanced vertex attribute.


    shader = get_shader("batched.glsl");
    otherquad = get_shader("other.glsl");
}


void BScene::on_update(float dt) {
    close = engine::is_key_pressed(GLFW_KEY_ESCAPE);
    clear_buffers(glm::vec4(0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glm::vec2 view_size = manager->main_window.size();
    //engine::rescale_camera_to_window(player.get_component<engine::CameraComp>(), manager->main_window);

    render_batched(shader, VAO, 100, 4, 6);
    render_quad(glm::vec3(0.0f), glm::vec3(1.0f,0.0f,1.0f));

    manager->main_window.swap_and_poll();
}


void BScene::on_end() {
}

bool BScene::should_close() {
    return glfwWindowShouldClose(manager->main_window) || close ;
}


void BScene::render_quad(glm::vec3 position, glm::vec3 color) {
    glUseProgram(otherquad);
    glBindVertexArray(VAO);

    set_shader_f(otherquad, "time", glfwGetTime());
    set_shader_v3(otherquad, "position", position);
    set_shader_v3(otherquad, "color", color);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}



void render_batched(const Shader& shader, const unsigned int& VAO, size_t num, size_t v_num = 6, size_t i_num = 0) {
    glUseProgram(shader);
    glBindVertexArray(VAO);
    if (0 == i_num)
        glDrawArraysInstanced(GL_TRIANGLES, 0, v_num, num); // 100 triangles of 6 vertices each
    else
        glDrawElementsInstanced(GL_TRIANGLES, i_num, GL_UNSIGNED_INT, 0, num);
    glBindVertexArray(0);
}


