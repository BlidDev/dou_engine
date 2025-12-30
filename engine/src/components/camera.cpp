#include "components/camera.h"
#include "formatting.h"
#include "util.h"

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
        up = glm::vec3( 0.0f, 1.0f, 0.0f);       
        fovy = 90.0f;                            
        projection_mode = CameraProjection::Perspective;
        f_pitch = 0.0f;
        f_yaw = 0.0f;
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


    float CameraComp::set_pitch(float pitch, float max) {
        f_pitch = wrap_angle(pitch, max -  360.0f, max);
        return f_pitch;
	}

    float CameraComp::set_yaw(float yaw, float max) {
        f_yaw = wrap_angle(yaw, max -  360.0f, max);
        return f_yaw;
	}


    glm::mat4 CameraComp::get_view(glm::vec3 view_pos) {
        return glm::lookAt(view_pos, get_camera_target(*this, view_pos), up);
    }

    void CameraComp::log() {
        DU_CORE_INFO("Camera:");
        DU_CORE_INFO("\tUp: {}", up);
        DU_CORE_INFO("\tOrientation: ({}, {})", f_yaw, f_pitch);
        DU_CORE_INFO("\tFovy: {}", fovy);
        DU_CORE_INFO("\tProj: {}", (int)projection_mode);
    }

    CameraBuilder::CameraBuilder() {
        camera = {};
    }

    CameraBuilder& CameraBuilder::pitch(float pitch) {
        camera.set_pitch(pitch);
        return *this;
    }

    CameraBuilder& CameraBuilder::yaw(float yaw) {
        camera.set_yaw(yaw);
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
        DU_ASSERT(!camera.present_shader, "present_shader not called before building CameraComp or CameraBuilder given is invalid");
        return camera;
    }

    glm::vec3 get_camera_dir(const CameraComp& camera) {
        float pitch = glm::radians(camera.pitch());
        float yaw =   glm::radians(camera.yaw());

        return {
            cos(yaw)*cos(pitch),
            sin(pitch),
            sin(yaw)*cos(pitch)
        };
    }

    glm::vec3 get_camera_target(const CameraComp& camera, const glm::vec3& position) {
        return get_camera_dir(camera) + position;
    }


    glm::vec3 get_camera_forward(const CameraComp& camera) {
        return get_camera_dir(camera);
    }

    glm::vec3 get_camera_right(const CameraComp& camera) {
        glm::vec3 forward = get_camera_dir(camera);
        glm::vec3 tmp_up = glm::normalize(camera.up);

        return glm::normalize(glm::cross(forward, tmp_up));
    }

    glm::vec3 get_camera_flat_forward(const CameraComp& camera) {
        glm::vec3 flat_forward = get_camera_dir(camera);
        flat_forward.y = 0.0f;
        return glm::normalize(flat_forward);
    }

    void rescale_camera_to_window(CameraComp& camera, Window& window) {
        glm::vec2 size = window.size();
        if (camera.framebuffer.last_scale != size) {
            rescale_framebuffer(camera.framebuffer, size.x, size.y);
        }
    }

    void camera_pitch(CameraComp& camera, float delta, bool lock) {
        float desired = camera.pitch() + delta;
        if (lock)
            desired = std::clamp(desired, -89.9f, 89.9f);
        camera.set_pitch(desired);
	}

    void camera_yaw(CameraComp& camera, float delta) {
        camera.set_yaw(camera.yaw() + delta);
	}


    void handle_mouse_delta(CameraComp& camera, glm::vec2 delta, bool lock) { 
        camera_yaw(camera, delta.x);
        camera_pitch(camera, -delta.y, lock);
    }
}
