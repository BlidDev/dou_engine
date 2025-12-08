#include "components/camera.h"

namespace engine {

    void make_framebuffer(Frambuffer& fb, size_t w, size_t h) {
        glGenFramebuffers(1, &fb.handler);
        glBindFramebuffer(GL_FRAMEBUFFER, fb.handler);


        glGenTextures(1, &fb.texture);
        glBindTexture(GL_TEXTURE_2D, fb.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.texture, 0);
       
        glGenRenderbuffers(1, &fb.rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, fb.rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h); 
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb.rbo); 
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            DU_CORE_ERROR("Could not make frame buffer for editor view}");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        fb.last_scale = {w,h};
    }



    void rescale_framebuffer(Frambuffer& fb, size_t w, size_t h) {
        glBindTexture(GL_TEXTURE_2D, fb.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.texture, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, fb.rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb.rbo);

        fb.last_scale = {w,h};
    }

    CameraComp::CameraComp() {
        target = glm::vec3(0.0f, 0.0f, 1.0f);    // Camera looking at point
        up = glm::vec3( 0.0f, 1.0f, 0.0f);        // Camera up vector (rotation towards target)
        fovy = 90.0f;                              // Camera field-of-view Y
        projection_mode = CameraProjection::Perspective;
        last_pos = glm::vec3(0.0f, 0.0f, 0.0f );
        max_distance = 30.0f;
        framebuffer = {};
        present_shader = {};
    }

    glm::mat4 CameraComp::get_projection(glm::vec2 view_size) {
        switch (projection_mode) {
            case CameraProjection::Perspective: 
                return glm::perspective(glm::radians(fovy),
                                        view_size.x / view_size.y,
                                        0.1f, 100.0f);
            case CameraProjection::Orthographic:
                return glm::ortho(0.0f, view_size.x, 0.0f, view_size.y, 0.1f, 100.0f);
            default: DU_ASSERT(true, "Unkown projection mode {}", (int)projection_mode);
        }
    }

    glm::mat4 CameraComp::get_view(glm::vec3 view_pos) {
        update_camera_target(*this, view_pos);
        return glm::lookAt(view_pos, target, up);
    }

    void update_camera_target(CameraComp& camera, glm::vec3 position) {
        glm::vec3 delta = position - camera.last_pos; 
        camera.target += delta; 
        camera.last_pos = position;
    }



    void CameraComp::log() {
        DU_CORE_INFO("Camera:");
        DU_CORE_INFO("\tUp: ({}, {}, {})", up.x, up.y, up.z);
        DU_CORE_INFO("\tTarget: ({}, {}, {})", target.x, target.y, target.z);
        DU_CORE_INFO("\tFovy: {}", fovy);
        DU_CORE_INFO("\tProj: {}", (int)projection_mode);
    }

    CameraBuilder::CameraBuilder() {
        camera = {};
    }

    CameraBuilder& CameraBuilder::target(glm::vec3 target) {
        camera.target = target;
        return *this;
    }

    CameraBuilder& CameraBuilder::up(glm::vec3 up) {
        camera.up = up;
        return *this;
    }

    CameraBuilder& CameraBuilder::fovy(float fovy) {
        camera.fovy = fovy;
        return *this;
    }

    CameraBuilder& CameraBuilder::projection_mode(CameraProjection projection) {
        camera.projection_mode = projection;
        return *this;
    }

    CameraBuilder& CameraBuilder::max_distance(float distance) {
        camera.max_distance = distance;
        return *this;
    }

    CameraBuilder& CameraBuilder::framebuffer_size(size_t w, size_t h) {
        make_framebuffer(camera.framebuffer, w, h);
        return *this;
    }

    CameraBuilder& CameraBuilder::present_shader(Shader& shader) {
        camera.present_shader = shader;
        return *this;
    }

    CameraComp CameraBuilder::build() {
        DU_ASSERT(!camera.framebuffer, "framebuffer_size not called before building CameraComp");
        DU_ASSERT(!camera.present_shader, "present_shader not called before building CameraComp or CameraBuilder given is ivalid");
        return camera;
    }


    glm::vec3 get_camera_dir(glm::vec3& target, glm::vec3& position) {
        glm::vec3 dir = glm::normalize(target - position);
        return dir;
    }

    glm::vec3 get_flat_forward(glm::vec3& target, glm::vec3& position) {
        glm::vec3 dir = glm::normalize(target - position);
        float yaw = atan2(dir.z, dir.x);

        glm::vec3 flat_forward = glm::vec3(cos(yaw), 0.0f, sin(yaw));

        return flat_forward;
    }


    void rescale_camera_to_window(CameraComp& camera, Window& window) {
        glm::vec2 size = window.size();
        if (camera.framebuffer.last_scale != size) {
            rescale_framebuffer(camera.framebuffer, size.x, size.y);
        }
    }
}
